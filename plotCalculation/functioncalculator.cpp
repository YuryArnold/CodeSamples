#include "functioncalculator.h"

FunctionCalculator::FunctionCalculator(QObject *parent) : QObject(parent)
{
    pauseCalcuation = false;
    breakState = false;
}

FunctionCalculator::~FunctionCalculator()
{

}

void FunctionCalculator::setFunctionParametersSlot(FunctionParameters _params)
{
    params = _params;
    this->calculateNumberOfSteps();
}

void FunctionCalculator::setFunction(std::function<double (double& , FunctionParameters &)> _calculationCore)
{
    calculationCore = _calculationCore;
}

void FunctionCalculator::pauseCalculation(bool _state)
{
     pauseCalcuation = _state;
}

bool FunctionCalculator::isPaused()
{
    return pauseCalcuation;
}

void FunctionCalculator::calculateNumberOfSteps()
{
    double interval = params.stop - params.start;
    params.endInd = interval/params.step;
}


void FunctionCalculator::calculculateFunction()
{
    double result;
    double x = 0;
    int duringIndTmp;
    for(duringIndTmp = params.startInd; duringIndTmp < params.endInd; ++duringIndTmp){
        if(breakState){
            params.state = stateOfCalculation::over;
            emit setLoadedParametrs(params);
            return;
        } else if(pauseCalcuation){
            params.startInd = duringIndTmp;
            params.state = stateOfCalculation::pause;
            emit setLoadedParametrs(params);
            return;
        }
        x = duringIndTmp * params.step + params.start;
        progress = 100*duringIndTmp/params.endInd;
        emit calculationProgress(progress);
        result = calculationCore(x,params);
        emit emitCalculatedPoint(x,result);
        if(duringIndTmp%100)
            QThread::msleep(10);
    }
    pauseCalcuation = false;
    params.state = stateOfCalculation::over;
    emit setLoadedParametrs(params);
    emit calculationAccomplished();
}

void FunctionCalculator::setBreak()
{
    breakState = true;
    pauseCalcuation = false;
}

void FunctionCalculator::reset()
{
    pauseCalcuation = false;
    breakState = false;
}
