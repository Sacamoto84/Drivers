
# Макрос создания структуры описания экрана типа спрайт
Спрайт создается в памяти, и не выводит никуда изображение, но к нему применимы все методы работы с экраном и его можно обьеденить с текущим буффером другого экрана
 
Пока реализован только 16 бит спрайт
 
```cpp
#define CREATE_SPRITE16(name, x , y) u16 name##_buffer16[x * y + 4];\
TFT_LCD_t name={x , y, SPRITE_RAM, 16, NULL , NULL, 0, &name##_buffer16[0],};
```

```cpp
TFT Sprite; <- Это Экран спрайта

//Выделение памяти
CREATE_SPRITE16(S1, 32 , 32);
...
Sprite.init(&S1);
```

Со спрайтами можно производить некоторые трансформации

## Поместить спрайт на LCD
```
tft.copy(&Sprite, -32, 20);
tft.copyTr(&Cursor, TS[1], TS[2], BLACK);
```

## Вращение

```
Sprite.setPivot(-20, 15); //Точка вращения
tft.setPivot(135 / 2, 240 / 2);

tft.pushRotated(&Sprite, angle, BLACK);
tft.pushRotated(&Sprite, angle, WHITE);

tft.copy(&Sprite, 20, 20);
```

## Скроллинг
```
Sprite.SetPixel(25, graphVal, BLUE);
Sprite.scroll(-1, 0);
```