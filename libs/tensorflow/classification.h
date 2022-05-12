#ifndef _LIBS_TENSORFLOW_CLASSIFICATION_H_
#define _LIBS_TENSORFLOW_CLASSIFICATION_H_

#include <limits>
#include <vector>

#include "third_party/tflite-micro/tensorflow/lite/micro/micro_interpreter.h"

namespace coral::micro {
namespace tensorflow {

struct Class {
    int id;
    float score;
};

std::vector<Class> GetClassificationResults(
    const float* scores, ssize_t scores_count,
    float threshold = -std::numeric_limits<float>::infinity(),
    size_t top_k = std::numeric_limits<size_t>::max());

std::vector<Class> GetClassificationResults(
    tflite::MicroInterpreter* interpreter,
    float threshold = -std::numeric_limits<float>::infinity(),
    size_t top_k = std::numeric_limits<size_t>::max());

bool ClassificationInputNeedsPreprocessing(const TfLiteTensor& input_tensor);

bool ClassificationPreprocess(TfLiteTensor* input_tensor);

}  // namespace tensorflow
}  // namespace coral::micro

#endif  // _LIBS_TENSORFLOW_CLASSIFICATION_H_
