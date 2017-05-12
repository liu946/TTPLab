//
// Created by liu on {% DateTime %}.
//

#ifndef PROJECT_TRAINSTNLSTM_H
#define PROJECT_TRAINSTNLSTM_H

#include "process/{% ModelTrainer %}.h"
#include "../config/{% Solution %}{% Project %}Config.h"
#include "../structure/{% Project %}Sample.h"
#include "../model/{% Solution %}{% Project %}Model.h"
#include "extractor/ConverterFileReader.h"
#include "../extractor/ConventerDataTo{% Project %}Sample.h"
#include "extractor/ExtractorFileToWordEmb.h"


class Train{% Solution %}{% Project %} : public {% ModelTrainer %}<{% Solution %}{% Project %}TrainConfig, {% Project %}Sample> {
  {% Solution %}{% Project %}TrainConfig &config;
  {% Solution %}{% Project %}Model model;
public:
  Train{% Solution %}{% Project %}({% Solution %}{% Project %}TrainConfig &config)
          : LabelModelSGDSeqTrainer(config, model),
            config(config), model(config)
          {}

  void init() {
    // todo prepair data code

    // todo 1. init samples (to fill 'trainSamples' 'devSamples')
    //    initSample(trainSamples, config.training_data);
    //    initSample(devSamples, config.dev_data);

    // todo 2. init Dict (pass trainSample to fill Dict)
    //    model.registerWordChar(trainSamples);

    model.init(); // init model size
    if (!model.load() && config.embeding != "")
      initEmbedding();

  }

private:
  void initSample(vector<{% Project %}Sample> & samples, string file) {
    vector<DataFileName> fileName = {file};
    extractor::ConverterFileReader fileReader;
    fileReader.init(fileName);
    fileReader.run();

    ConverterDataTo{% Project %}Sample conv_toSample;
    conv_toSample.init(fileReader.getResult());
    conv_toSample.run();
    samples = conv_toSample.getResult();
  }

  void initEmbedding() {
    ExtractorFileToWordEmb reader;
    reader.init(config.embeding);
    unordered_map<string, vector<float> > emb = reader.run();
    model.initEmbedding(emb);
  }

};


#endif //PROJECT_TRAINSTNLSTM_H
