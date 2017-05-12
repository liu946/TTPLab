//
// Created by liu on {% DateTime %}.
//

#ifndef {% Solution %}_CONVENTERDATATO_{% Project %}_SAMPLE_H
#define {% Solution %}_CONVENTERDATATO_{% Project %}_SAMPLE_H

#include "extractor/Converter.h"
#include "structure/{% DataFormat %}.h"
#include "../structure/{% Project %}Sample.h"
using namespace extractor;

class ConverterDataTo{% Project %}Sample : public Converter<{% DataFormat %}, {% Project %}Sample>{
  int windowSize;
public:
  ConverterDataTo{% Project %}Sample() {}

  virtual void convert({% DataFormat %} &t1) {
    //todo insert code
  }

  virtual void afterConvert() {

    //todo insert code
  }

};

#endif //{% Solution %}_CONVENTERDATATO_{% Project %}_SAMPLE_H
