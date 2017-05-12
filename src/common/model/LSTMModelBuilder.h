//
// Created by liu on 2017/2/20.
//

#ifndef TTPLAB_LSTMMODELBUILDER_H
#define TTPLAB_LSTMMODELBUILDER_H

#include "./ModelBuilder.h"
#include "dynet/nodes.h"
#include "dynet/dynet.h"
#include "dynet/training.h"
#include "dynet/timing.h"
#include "dynet/rnn.h"
#include "dynet/gru.h"
#include "dynet/lstm.h"
#include "dynet/dict.h"
#include <dynet/expr.h>
#include <dynet/model.h>
#include "vector"

using namespace dynet;
using namespace dynet::expr;
using namespace std;

class LSTMModelBuilder : public ModelBuilder<vector<Expression>, vector<Expression>> {
  unsigned layers;
  unsigned inputDim;
  unsigned outputDim;
  LSTMBuilder lstm;
  Parameter begin;
  Parameter end;
public:
  LSTMModelBuilder(unsigned layers, unsigned inputDim, unsigned outputDim) :
          layers(layers),
          inputDim(inputDim),
          outputDim(outputDim)
  {}

  void setLayers(unsigned int layers) {
    LSTMModelBuilder::layers = layers;
  }

  void setInputDim(unsigned int inputDim) {
    LSTMModelBuilder::inputDim = inputDim;
  }

  void setOutputDim(unsigned int outputDim) {
    LSTMModelBuilder::outputDim = outputDim;
  }

  using ModelBuilder::init;
  virtual void init(Model &model, bool initBeginEnd = true) {
    lstm = LSTMBuilder(layers, inputDim, outputDim, model);
    if (initBeginEnd) {
      begin = model.add_parameters({inputDim});
      end = model.add_parameters({inputDim});
    }
  }

  virtual void init(Model &model, Parameter& begin, Parameter& end) {
    this->begin = begin;
    this->end = end;
    init(model, false);
  }

  void newGraph(ComputationGraph &cg) {
    lstm.new_graph(cg);
  }
  void startNewSequence(vector<Expression> h_0 = {}) {
    lstm.start_new_sequence();
  }

  void dropOut(float d) {
    if (d > 1e-6) {
      lstm.set_dropout(d);
    } else {
      lstm.disable_dropout();
    }
  }

  void disableDropOut() {
    lstm.disable_dropout();
  }

  /**
   *
   * @param hg
   * @param inputList
   * @return
   *
   * begin -> [1 -> 2 -> ... -> last] -> end
   *           |    |   |...|    |
   *           V    V   V   V    V
   *          [1 -> 2 -> ... -> last] return this out put
   */
  virtual vector<Expression> forward(dynet::ComputationGraph &hg, const vector<Expression>& inputList) {
    vector<Expression> res;
    lstm.add_input(parameter(hg, begin));
    for (int i = 0; i < inputList.size(); ++i) {
      res.push_back(lstm.add_input(inputList[i]));
    }
    lstm.add_input(parameter(hg, end));
    return res;
  }

  /**
   * this is used for backward lstm in BiLSTM
   * note : use forward or use backward only! Use both is logically wrong!
   * @param hg
   * @param inputList
   * @return
   *
   * begin <- [1 <- 2 <- ... <- last] <- end
   *           |    |   |...|    |
   *           V    V   V   V    V
   *          [1 -> 2 -> ... -> last] return this out put
   */

  virtual vector<Expression> backward(dynet::ComputationGraph &hg, const vector<Expression>& inputList) {
    vector<Expression> res(inputList.size());
    lstm.add_input(parameter(hg, end));
    for (int i = inputList.size() - 1; i >= 0; --i) {
      res[i] = lstm.add_input(inputList[i]);
    }
    lstm.add_input(parameter(hg, begin));
    return res;
  }

  /**
   *
   * @param hg
   * @param inputList
   * @return
   *
   * begin -> [1 -> 2 -> ... -> last] -> end
   *                                     |
   *                                     V
   *                                    EXP return this out put
   */
  virtual Expression forwardBack(dynet::ComputationGraph &hg, vector<Expression>& inputList) {
    lstm.add_input(parameter(hg, begin));
    for (int i = 0; i < inputList.size(); ++i) {
      lstm.add_input(inputList[i]);
    }
    lstm.add_input(parameter(hg, end));
    return lstm.back();
  }

  /**
   *
   * @param hg
   * @param inputList
   * @return
   *
   * begin <- [1 <- 2 <- ... <- last] <- end
   *   |
   *   V
   *  EXP return this out put
   */
  virtual Expression backwardBack(dynet::ComputationGraph &hg, vector<Expression>& inputList) {
    lstm.add_input(parameter(hg, end));
    for (int i = inputList.size() - 1; i >= 0; --i) {
      lstm.add_input(inputList[i]);
    }
    lstm.add_input(parameter(hg, begin));
    return lstm.back();
  }

  /**
   * Forward by order
   * tricky foo
   */
  virtual vector<Expression> forwardByOrder(dynet::ComputationGraph &hg, vector<Expression>& inputList, vector<int>& order) {
    vector<Expression> res;
    lstm.add_input(parameter(hg, begin));
    for (int i = 0; i < order.size(); ++i) {
      res.push_back(lstm.add_input(inputList[order[i]]));
    }
    lstm.add_input(parameter(hg, end));
    return res;
  }

  virtual Expression forwardBackByOrder(dynet::ComputationGraph &hg, vector<Expression>& inputList, vector<int>& order) {
    lstm.add_input(parameter(hg, begin));
    for (int i = 0; i < order.size(); ++i) {
      lstm.add_input(inputList[order[i]]);
    }
    return lstm.back();
  }

};


#endif //TTPLAB_LSTMMODELBUILDER_H
