//
// Created by liu on {% DateTime %}.
//

#ifndef PROJECT_STNLSTM_H
#define PROJECT_STNLSTM_H

#include <base/debug.h>
#include <model/SeqLabelModel.h>
#include "../config/{% Solution %}{% Project %}Config.h"
#include "../structure/{% Project %}Sample.h"

// model builders
#include <model/LookupModelBuilder.h>
#include <model/BiLSTMModelBuilder.h>
#include <model/AffineTransformModelBuilder.h>


class {% Solution %}{% Project %}Model : public SeqLabelModel<{% Project %}Sample> {
  {% Solution %}{% Project %}BaseConfig & config;
  base::Debug debug;
  // todo define ModelBuilders

  // todo define
    enum Look {
      WORD = 0,
      CHAR,
      ALL
    };
public:
  {% Solution %}{% Project %}Model({% Solution %}{% Project %}BaseConfig &config) :
    SeqLabelModel(config), config(config), debug("{% Solution %}{% Project %}Model") { }

  void registerDict(vector<{% Project %}Sample>& trainSamples) {
    dict.resize(Look::ALL);
//    for (int j = 0; j < samples.size(); ++j) {
//      for (int k = 0; k < samples[j].size(); ++k) {
//        dict[WORD].convert(samples[j].getWord(k).getRawWord());
//        dict[CHAR].convert(samples[j].getWord(k).getFirstChar());
//      }
//    }
    freezeDict();
    dict[WORD].set_unk(UNK_WORD);
    dict[CHAR].set_unk(UNK_WORD);
  }

  void initEmbedding(unordered_map<string, vector<float> > & emb) {
    // todo init embedings
    // initParameter(emb, word_lookup, dict[WORD]);
  }

  void init() {
    // todo init the builders

    // todo builders.init(model)

  }

  virtual vector<Expression> label(ComputationGraph &hg, {% Project %}Sample &samples) {
    // todo express your model
    vector<Expression> ans;
    return ans;
  }

  virtual Expression
  ExtractError(ComputationGraph &hg, vector<Expression> &adists, {% Project %}Sample &samples, Performance &perf) {
    // todo define your loss
    vector<Expression> err;

    return -sum(err);
  }

};


#endif //PROJECT_STNLSTM_H
