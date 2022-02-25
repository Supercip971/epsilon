#ifndef PROBABILITY_MODELS_STATISTIC_GOODNESS_TEST_H
#define PROBABILITY_MODELS_STATISTIC_GOODNESS_TEST_H

#include "chi2_test.h"

namespace Probability {

class GoodnessTest final : public Chi2Test {
public:
  GoodnessTest();
  CategoricalType categoricalType() const override { return CategoricalType::GoodnessOfFit; }
  I18n::Message title() const override { return I18n::Message::InputGoodnessControllerTitle; }
  int numberOfStatisticParameters() const override { return k_maxNumberOfRows * k_maxNumberOfColumns; }
  void setGraphTitle(char * buffer, size_t bufferSize) const override;
  void setResultTitle(char * buffer, size_t bufferSize, bool resultIsTopPage) const override;

  // Inference
  bool validateInputs() override;
  // Statistic
  int numberOfResults() const override { return 2; }
  void computeTest() override;

  // Chi2Test
  void setParameterAtPosition(double p, int row, int column) override;
  double parameterAtPosition(int row, int column) const override;
  bool authorizedParameterAtPosition(double p, int row, int column) const override;
  bool deleteParameterAtPosition(int row, int column) override;
  void recomputeData() override;
  int maxNumberOfColumns() const override { return k_maxNumberOfColumns; };
  int maxNumberOfRows() const override { return k_maxNumberOfRows; };

  void setDegreeOfFreedom(double degreeOfFreedom) { m_degreesOfFreedom = degreeOfFreedom; }
  /* Return the DegreesOfFreedom computed from the numberOfValuesPairs. Actual
   * statistic's degree of freedom may differ because it can be overridden by
   * the user. */
  int computeDegreesOfFreedom() { return numberOfValuePairs() - 1; }
  int computeNumberOfRows() const;
private:
  constexpr static int k_maxNumberOfRows = 10;
  constexpr static int k_maxNumberOfColumns = 2;

  // Statistic
  bool authorizedParameterAtIndex(double p, int i) const override;

  // Chi2Test
  double expectedValue(int index) const override;
  double observedValue(int index) const override;
  int numberOfValuePairs() const override;

  double * parametersArray() override { return m_input; }
  void setExpectedValue(int index, double value);
  void setObservedValue(int index, double value);
  int locationToTableIndex(int row, int column) const;

  double m_input[k_maxNumberOfRows * k_maxNumberOfColumns];
};

}  // namespace Probability

#endif /* PROBABILITY_MODELS_STATISTIC_GOODNESS_TEST_H */