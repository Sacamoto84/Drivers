### ✨ [top](../README.md)

```
Fill   (u16 color)
Fill1  (u16 color)
Fill4  (u16 color)
Fill8  (u16 color)
Fill16 (u16 color)
```

```
SetPixel   (i32 x, i32 y, u16 color)
SetPixel1  (i32 x, i32 y, u16 color)
SetPixel4  (i32 x, i32 y, u16 color)
SetPixel8  (i32 x, i32 y, u16 color)
SetPixel16 (i32 x, i32 y, u16 color)
```

------------------------------------------------------------
Примитивы
------------------------------------------------------------
```
Line       (i32 x0, i32 y0, i32 x1, i32 y1, ui16 c)
LineV      (i32 X, i32 Y1, i32 Y2, u16 color)        Вертикальная линия   Y2>=Y1   [*]
LineH      (i32 Y, i32 X1, i32 X2, u16 color)        Горизонтальная линия X2>=X1   [*]
LineH16    (i32 Y, i32 X1, i32 X2, u16 color)        Горизонтальная линия X2>=X1   [16]
LineHW     (i32 x, i32 y, i32 w, u16 color)          Горизонтальная линия w-ширина
LineHW16   (i32 x, i32 y, i32 w, u16 color)          Горизонтальная линия w-ширина [16]

LineMoveXY (i32 x, i32 y)                            Установка начала линии, только координата 
LineMoveTo (i32 x, i32 y, u16 c)                     Линия к точке xy от прошлой  координаты [*]
```

```
Rectangle(i32 x, i32 y, u16 w, u16 h, u16 c)
RectangleFilled(u16 x, u16 y, u16 w, u16 h, u16 c)
InvertRectangle(u16 x, u16 y, u16 w, u16 h)
ChangeColorRectangle(i32 x, i32 y, u32 w, i32 h, u16 sC, u16 dC) Замена цвета sC -> dC
```


```
Circle       (i16 x0, i16 y0, i16 r, u16 c)
CircleFilled (i16 x0, i16 y0, i16 r, u16 c)
```

```
Triangle       (u16 x1, u16 y1, u16 x2, u16 y2,	u16 x3, u16 y3, u16 color)
TriangleFilled (i16 x1, i16 y1, i16 x2, i16 y2,	i16 x3, i16 y3, u16 color)
```

```
Только 16 бит
//tft.Gradient_Vertical(0, 0, 239, 239, 0x26, 0x4d, 0x59, 0x43, 0x97, 0x8d);
//tft.Gradient_Vertical(0, 0, 239, 239, 0x68, 0x82, 0xa0, 0x2c, 0x69, 0x75);
//tft.Gradient_Vertical(0, 0, 239, 239, 0x26, 0x4d, 0x59, 0x2c, 0x69, 0x75);

Gradient_Vertical(u32 x0, u32 y0, u32 w, u32 h, u8 otR, u8 otG, u8 otB, u8 doR, u8 doG, u8 doB) [16]
```
------------------------------------------------------------

Для работы фонтов
------------------------------------------------------------
```
SetFontColor (u16 Color, u16 BColor)
SetColor     (u16 Color)
SetBColor    (u16 BColor)
GotoXY       (i16 x, i16 y)
```