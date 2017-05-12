//
// Created by liu on {% DateTime %}.
//

#ifndef PROJECT_PREDSTNLSTM_H
#define PROJECT_PREDSTNLSTM_H

#include <process/LabelModelSGDSeqPredictor.h>
#include <extractor/ConverterFileReader.h>
#include "../config/{% Solution %}{% Project %}Config.h"
#include "../structure/{% Project %}Sample.h"
#include "../model/{% Solution %}{% Project %}Model.h"
#include "../extractor/ConventerDataTo{% Project %}Sample.h"

using namespace std;

class Pred{% Solution %}{% Project %} : public LabelModelSGDSeqPredictor<{% Solution %}{% Project %}PredConfig, {% Project %}Sample>{
  {% Solution %}{% Project %}PredConfig & config;
  {% Solution %}{% Project %}Model model;
public:
  Pred{% Solution %}{% Project %}({% Solution %}{% Project %}PredConfig &config)
          : LabelModelSGDSeqPredictor(config, model),
            config(config), model(config) {}

  virtual void init() {

    // todo prepair
    // todo 1. prepair testSamples (to fill 'testSamples')
    initSample(testSamples, config.test_data);
    model.loadDict(); // load dict
    model.init(); // init parameters
    model.load(); // load model
  }

  virtual void extractResult() {
    // todo extract Result (use predict in testSamples)
  }

private:

  void initSample(vector<{% Project %}Sample>& samples, string file) {
    vector<extractor::DataFileName> fileName = {file};
    extractor::ConverterFileReader fileReader;
    fileReader.init(fileName);
    fileReader.run();

    ConverterDataTo{% Project %}Sample conv_toSample;
    conv_toSample.init(fileReader.getResult());
    conv_toSample.run();
    samples = conv_toSample.getResult();
  }
};


#endif //PROJECT_PREDSTNLSTM_H
