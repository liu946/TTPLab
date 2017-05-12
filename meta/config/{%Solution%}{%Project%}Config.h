//
// Created by liu on {% DateTime %}.
//

#ifndef {% Solution %}_{% Project %}_CONFIG_H
#define {% Solution %}_{% Project %}_CONFIG_H

#include "config/ModelConf.h"

class {% Solution %}{% Project %}BaseConfig : public virtual ModelConf {
public:
  unsigned word_dim;
  {% Solution %}{% Project %}BaseConfig(string confName = "Configuration"): ModelConf(confName) {
    registerConf<unsigned>("word_dim"          , UNSIGNED, word_dim         , "word_dim", 100);

  }
};

class {% Solution %}{% Project %}TrainConfig : public virtual {% Solution %}{% Project %}BaseConfig, public virtual LabelModelTrainerConf {
public:
  string embeding;
  {% Solution %}{% Project %}TrainConfig(string confName = "Configuration"):
          {% Solution %}{% Project %}BaseConfig(confName),
          LabelModelTrainerConf(confName)
  {
    registerConf<string>  ("embeding" , STRING,   embeding , "embeding", "");
  }
};

class {% Solution %}{% Project %}PredConfig : public virtual {% Solution %}{% Project %}BaseConfig, public virtual LabelModelPredictorConf {
public:
  {% Solution %}{% Project %}PredConfig(string confName = "Configuration"):
          {% Solution %}{% Project %}BaseConfig(confName),
          LabelModelPredictorConf(confName)
  { }
};

#endif //{% Solution %}_{% Project %}_CONFIG_H
