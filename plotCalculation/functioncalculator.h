#ifndef FUNCTIONCALCULATOR_H
#define FUNCTIONCALCULATOR_H
#include <QObject>
#include <QVector>
#include <QThread>
#include <functional>
#include <atomic>
#include "sharedstructures.h"

class FunctionCalculator : public QObject
{
    Q_OBJECT
public:
    explicit FunctionCalculator(QObject *parent = nullptr);
    virtual ~FunctionCalculator();

    void setFunction(std::function<double(double& x, FunctionParameters& _params)> _calculationCore);
    void pauseCalculation(bool _state);
    bool isPaused();


private:
    void calculateNumberOfSteps();

signals:
    void endOfCalculation();
    void emitCalculatedPoint(double,double);
    void unpauseCalculationFromInside();
    void calculationProgress(int);
    void setLoadedParametrs(FunctionParameters _params);
    void calculationAccomplished();

public slots:
    void calculculateFunction();
    void setBreak();
    void setFunctionParametersSlot(FunctionParameters _params);
    void reset();

private:
    std::function<double(double& x, FunctionParameters& _params)> calculationCore;
    FunctionParameters params;
    int progress;
    std::atomic<bool> pauseCalcuation;
    std::atomic<bool> breakState;
};

#endif // FUNCTIONCALCULATOR_H
