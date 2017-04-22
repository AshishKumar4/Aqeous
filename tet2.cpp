#include "iostream"
#include "string.h"

using namespace std;

class rect
{
protected:
  float len;
  float bre;
public:
  void get();
  float area();
};

class screen : public rect
{
public:
  int cost_area;
  int cost_tube;
  int cost_speakers;

  screen()
  {
    get();
    cin>>cost_area>>cost_tube>>cost_speakers;
  }

  void ShowCost()
  {
    cout<<cost_tube+cost_speakers+(area()*cost_area);
  }
};

void rect::get()
{
  cin>>len>>bre;
}

float rect::area()
{
  return len*bre;
}

int main()
{
  screen* tv = new screen();
  tv->ShowCost();
  return 0;
}
