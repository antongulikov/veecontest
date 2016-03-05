//
// Created by scorpion on 01.03.16.
//

#pragma once

#include "vertex.h"

struct Action {
public:

    Action &getInstance() {
        static Action instance;
        return instance;
    }

    void setAction(actions type) {
        actionType = type;
    }

    void setTime(int time) {
        actionTime = time;
    }

    void setVertex(int v) {
        actionVertex = v;
    }

    void setFirst(int first) {
        firstP = first;
    }

    void setSecond(int second) {
        secondP = second;
    }

    string getAction() {
        string result = actionRepr[actionType];
        if (actionType != actions::end)
            result += " " + to_str(actionTime);
        switch (actionType) {
            case actions::move: {
                result += " " + to_str(actionVertex);
                break;
            }
            case actions::pick1: {
                result += " " + to_str(firstP);
                break;
            }
            case actions::drop1: {
                result += " " + to_str(firstP);
                break;
            }
            case actions::pick2: {
                result += " " + to_str(firstP);
                result += " " + to_str(secondP);
                break;
            }
            case actions::drop2: {
                result += " " + to_str(firstP);
                result += " " + to_str(secondP);
                break;
            }
        }
        return result;
    }


public:
    Action() { }

    string actionRepr[6] = {"move", "pick1", "pick2", "drop1", "drop2", "end"};

    string to_str(int value) {
        string ret = "";
        if (value == 0)
            return "0";
        while (value > 0) {
            ret.push_back('0' + (value % 10));
            value /= 10;
        }
        reverse(ret.begin(), ret.end());
        return ret;
    }

    actions actionType;
    int actionTime, actionVertex, firstP, secondP;
};