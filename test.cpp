#include "iostream"
#include "string.h"
#include "math.h"
using namespace std;
class coin
{
  char name;
  int weight;
public:
  void get();
  int getWeight();
  bool check_Coin(char);
};

class sp_coin : public coin
{
  char name;
  int weight;
  int followers;
public:
  void get()
  {
    cin>>name>>weight>>followers;
  }
};

void coin::get()
{
  cin>>name>>weight;
}

int coin::getWeight()
{
  cin>>weight;
}

bool coin::check_Coin(char a)
{
  if(a == name)
    return true;
  return false;
}

sp_coin* getCoin(char a)
{

}

int main()
{
  int n;
  cin>>n;
  coin* or_c = new coin[n];
  for(int i = 0; i < n; i++)
  {
    or_c[i].get();
  }
  int m;
  cin>>m;
  sp_coin* sp_c = new sp_coin[n];
  for(int i = 0; i < n; i++)
  {
    sp_c[i].get();
  }

  int p1 = 0, p2 = 0;
  int* p = &p1;


  char input_string[4096];
  cin>>input_string;

  char* tmpstr = strtok(input_string, "$");
  for(int i = 0; tmpstr != NULL; i++)
  {
    for(int j = 0; j < strlen(tmpstr); j++)
    {
      (*p) += getCoin(tmpstr[j]).getWeight();
    }
  }
  return 0;
}
