//
// Created by liu on 2017/2/20.
//

#ifndef TTPLAB_BILSTMMODELBUILDER_H
#define TTPLAB_BILSTMMODELBUILDER_H

#include "./ModelBuilder.h"
#include "./LSTMModelBuilder.h"
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

class BiLSTMModelBuilder : public ModelBuilder<vector<Expression>, vector<Expression>> {
  unsigned layers;
  unsigned inputDim;
  unsigned outputDim;
  LSTMModelBuilder forwardLSTM;
  LSTMModelBuilder backwardLSTM;
  Parameter begin;
  Parameter end;
public:
  BiLSTMModelBuilder(unsigned layers = 0, unsigned inputDim = 0, unsigned outputDim = 0):
          layers(layers),
          inputDim(inputDim),
          outputDim(outputDim),
          forwardLSTM(layers, inputDim, outputDim),
          backwardLSTM(layers, inputDim, outputDim) { }

  void setLayers(unsigned int layers) {
    BiLSTMModelBuilder::layers = layers;
    forwardLSTM.setLayers(layers);
    backwardLSTM.setLayers(layers);
  }

  void setInputDim(unsigned int inputDim) {
    BiLSTMModelBuilder::inputDim = inputDim;
    forwardLSTM.setInputDim(inputDim);
    backwardLSTM.setInputDim(inputDim);
  }

  void setOutputDim(unsigned int outputDim) {
    BiLSTMModelBuilder::outputDim = outputDim;
    forwardLSTM.setOutputDim(outputDim);
    backwardLSTM.setOutputDim(outputDim);
  }

  virtual void init(Model &model) {
    assert(layers > 0);
    assert(inputDim > 0);
    assert(outputDim > 0);
    begin = model.add_parameters({inputDim});
    end = model.add_parameters({inputDim});
    forwardLSTM.init(model, begin, end);
    backwardLSTM.init(model, begin, end); // use backward function will handle this.
  }

  void dropOut(float d) {
    forwardLSTM.dropOut(d);
    backwardLSTM.dropOut(d);
  }

  void disableDropOut() {
    forwardLSTM.disableDropOut();
    backwardLSTM.disableDropOut();
  }

  void newGraph(ComputationGraph &cg) {
    forwardLSTM.newGraph(cg);
    backwardLSTM.newGraph(cg);
  }
  void startNewSequence(vector<Expression> h_0 = {}) {
    forwardLSTM.startNewSequence(h_0);
    backwardLSTM.startNewSequence(h_0);
  }
  /**
   *
   * @param hg
   * @param aClass
   * @return A 2*outputDim Dim Expression
   */
  virtual vector<Expression> forward(dynet::ComputationGraph &hg, const vector<Expression>& inputList) {
    vector<Expression> res;
    vector<Expression> fw = forwardLSTM.forward(hg, inputList);
    vector<Expression> bw = backwardLSTM.backward(hg, inputList);
    int size = (int) fw.size();
    for (int i = 0; i < size; ++i) {
      res.push_back(concatenate({fw[i], bw[i]}));
    }
    return res;
  }

  virtual Expression forwardBack(dynet::ComputationGraph &hg, vector<Expression> inputList) {
    return concatenate({
                               forwardLSTM.forwardBack(hg, inputList),
                               backwardLSTM.backwardBack(hg, inputList)
                       });
  }

  virtual Expression forwardBackBy2Order(dynet::ComputationGraph &hg, vector<Expression> inputList, vector<int> order1, vector<int> order2) {
    return concatenate({
                               forwardLSTM.forwardBackByOrder(hg, inputList, order1),
                               backwardLSTM.forwardBackByOrder(hg, inputList, order2)
                       });
  }

  virtual Expression forwardBackBy2Path(dynet::ComputationGraph &hg, vector<Expression> fwPath, vector<Expression> bwPath) {
    return concatenate({
                                forwardLSTM.forwardBack(hg, fwPath),
                                backwardLSTM.forwardBack(hg, bwPath)
                       });
  }
};


#endif //TTPLAB_BILSTMMODELBUILDER_H
