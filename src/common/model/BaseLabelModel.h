//
// Created by liu on 2017/2/22.
//

#ifndef PROJECT_BASELABELMODEL_H
#define PROJECT_BASELABELMODEL_H


#include "base/debug.h"
#include "dynet/dict.h"
#include <model/Model.h>
#include <dynet/model.h>
#include "dynet/expr.h"
#include "structure/Performance.h"
#include "structure/Prediction.h"
#include "config/ModelConf.h"
#include "Const.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <fstream>
#include "model/LookupModelBuilder.h"

using namespace dynet;
using namespace dynet::expr;

template <class SampleClass>
class BaseLabelModel : public model::Model  {
  ModelConf& config;
  base::Debug debug;
public:
  dynet::Model model;
  vector<dynet::Dict> dict;
  float dropout_rate = 0; // 默认关闭dropout

  BaseLabelModel(ModelConf &config) : config(config), debug("BaseLabelModel") { }

  void save() {
    ofstream out(config.model);
    boost::archive::text_oarchive oa(out);
    oa << dict; oa << model;
    out.close();
    debug.debug("model saved in '%s'", config.model.c_str());
  }

  bool load() {
    ifstream in(config.model);
    if (!in) {
      debug.debug("no model found in '%s'", config.model.c_str());
      return false;
    }
    model = dynet::Model();
    debug.debug("load model in '%s'", config.model.c_str());
    boost::archive::text_iarchive ia(in);
    ia >> dict; ia >> model;
    in.close();
    return true;
  }

  bool loadDict() {
    ifstream in(config.model);
    if (!in) {
      debug.debug("no model found in '%s'", config.model.c_str());
      return false;
    }
    debug.debug("load dict in '%s'", config.model.c_str());
    boost::archive::text_iarchive ia(in);
    ia >> dict;
    in.close();
    return true;
  }

  void setDropOut(float dropout_rate) {
    this->dropout_rate = dropout_rate;
  }

  void freezeDict() {
    for (int j = 0; j < dict.size(); ++j) {
      debug.debug("dict[%d] is frozen at size %u", j, dict[j].size());
      dict[j].freeze();
    }
  }

// 工具函数
public:
  void initParameter(unordered_map<string, vector<float> >& emb,
                     LookupModelBuilder & lookupParameter,
                     dynet::Dict & dict
  ) {
    double emb_size = emb.size();
    double word_size = dict.size();
    double fill_size = 0;
    for (unsigned j = 0; j < dict.size(); ++j) {
      if (emb.find(dict.convert(j)) != emb.end()) {
        lookupParameter.initialize(j, emb[dict.convert(j)]);
        fill_size ++;
      }
    }
    debug.debug("Fill lookup parameter(%lf) with emb(%lf), (%lf %lf%%) filled.", word_size, emb_size, fill_size, fill_size/word_size);
  }

protected:
/**
 * 统计信息，更新performance
 * @param perf
 * @param god  {keep the nil label = 0}
 * @param adist
 */
  void setPerf(Performance & perf, unsigned god, vector<float> adist) {
    // evaluate 概率最大标签概率 max_prob 和 概率最大标签 max_idx
    double max_prob = adist[0];
    unsigned max_idx = 0;
    for (unsigned i = 1; i < adist.size(); ++i) {
      if (adist[i] > max_prob) {
        max_prob = adist[i];
        max_idx = i;
      }
    }
    if (god == 0) perf.n_arg += 1;
    if (max_idx == 0) {
      perf.n_parg += 1;
      if (max_idx == god)
        perf.tp += 1;
    }
  }

/**
 * 输出预测比较统计
 */
  struct cmp_outcome {
    bool operator()(const pair<int, double>& lpr,
                    const pair<int, double>& rpr) const {
      return lpr.second > rpr.second;
    }
  };

  virtual Prediction extractPrediction(vector<float> probs) {
    Prediction prediction;
    for (unsigned i = 0; i < probs.size(); ++i) {
      prediction.push_back(make_pair(i, (double)probs[i]));
    }
    sort(prediction.begin(), prediction.end(), cmp_outcome());
    return prediction;
  }


  /**
 * 激活函数
 * @param expr
 * @return
 */
  virtual Expression activate(dynet::expr::Expression expr) {
    dynet::expr::Expression nl_hidden;
    if (config.activate == "tanh") {
      nl_hidden = tanh(expr);
    }else if (config.activate == "cube"){
      nl_hidden = cube(expr);
    } else {
      nl_hidden = rectify(expr);
    }
    if (dropout_rate > 1e-7) {
      nl_hidden = dropout(nl_hidden, dropout_rate);
    }
    return nl_hidden;
  }

  /**
  * 绑定两个列表中的每一项
  *
  * @param exprList1
  * @param exprList2
  * @return
  */
  virtual vector<Expression> concatenate(vector<Expression> exprList1, vector<Expression> exprList2) {
    assert(exprList1.size() == exprList2.size());
    vector<Expression> res;
    for (int i = 0; i < exprList1.size(); ++i) {
      res.push_back(dynet::expr::concatenate({exprList1[i], exprList2[i]}));
    }
    return res;
  }

/**
  * 列表按照顺序取出 Expression
  *
  * @param exprList1
  * @param exprList2
  * @param outOfIndex 用于ROOT的表示
  * @return
  */
  virtual vector<Expression> lookUpExprList(vector<Expression> exprList, vector<int>& indexList, Expression& outOfIndex) {
    vector<Expression> res;
    for (int i = 0; i < indexList.size(); ++i) {
      if (indexList[i] < 0) {
        res.push_back(outOfIndex);
      } else {
        res.push_back(exprList[indexList[i]]);
      }
    }
    return res;
  }
};


#endif //PROJECT_BASELABELMODEL_H
