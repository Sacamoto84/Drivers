#ifndef livedata_h
#define livedata_h

#include "main.h"
#include "macros.h"

//LivaData <float> xxx(&value);

//if (xxx.onchange())
//{ }


template <typename T>

class LiveData {
public:

  //Конструктор
  LiveData(T * in)
  {
	  value = in;
      lastValue =  ~(*value);
  }

  void SetValue(T in)  { *value = in; }
  T    GetValue(void)  { return * value; }

  //Событие возвращает true если произошло изменение
  bool onChange()  {
	  if (*value != lastValue)  {	  lastValue = *value;  return true;	  }

  }

  //Событие возвращает true если произошло изменение в сторону увеличения
  bool onChangeP()  {
	  if (*value > lastValue)   {	  lastValue = *value;  return true;	  }
	  else                      {     lastValue = *value;  return false;  }
  }

  //Событие возвращает true если произошло изменение в сторону уменьшения
  bool onChangeN()  {
	  if (*value > lastValue)   {	  lastValue = *value;  return true;	  }
	  else                      {     lastValue = *value;  return false;  }
  }


private:
  T * value;
  T   lastValue;

};










#endif
