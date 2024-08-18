#include "random.h"


int randomInt(int a, int b)
{
    static std::uniform_int_distribution<int> ri(0,INT_MAX);
    static std::default_random_engine generator(time(0));
    auto f=ri(generator);
    return f%(b-a+1)+a;
}

float randomFloat()
{
    static std::uniform_real_distribution<float> rf(0.0,1.0);
    static std::default_random_engine generator(time(0));
    return rf(generator);
}

float randomFloat(float a, float b)
{
    static std::uniform_real_distribution<float> rf(0.0,1.0);
    static std::default_random_engine generator(time(0));
    auto f=rf(generator);
    return f*a+(1.0f-f)*b;
}

Random::Random():randomInt(0,INT_MAX),generator(time(0))
{

}

void Random::SetSeed(int seed)
{
    generator=decltype(generator)(seed);
}

int Random::rand(int a, int b)
{
    auto f = randomInt(generator);
    return f % (b - a + 1) + a;
}
