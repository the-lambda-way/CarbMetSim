#pragma once

class HumanBody;


class AdiposeTissue
{
public:
    AdiposeTissue(HumanBody* body);

    void processTick();
    void setParams();

    void lipogenesis(double glucoseInMG);
    void consumeFat(double kcal);
    void addFat(double newFatInMG);

    double fat; // fat in body in grams

private:
    HumanBody* body;
};
