#pragma once

template <typename T>
class Observateur
{
public:
    virtual void update(const T &info) =0;
};
