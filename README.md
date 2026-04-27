# CSV pretty printer

Преобразует CSV файл ```data/input.csv```

Например:
```
Test field 1,Test field 2
test,123
long string test!,28.7
other text,3
```

В таблицу в формате псевдографики и записывающий её в файл ```data/output.txt```
```
+===================+==============+
| Test field 1      | Test field 2 |
+===================+==============+
| test              |          123 |
+-------------------+--------------+
| long string test! |         28.7 |
+-------------------+--------------+
| other text        |            3 |
+-------------------+--------------+
```

## 1. Собрать в папку build и запустить:

Создать папку, если нет:
```
mkdir build
```
---
```
cd build
```
```
cmake ..
```
```
make
```
```
 ./CSV_PrettyPrinter 
```

---

## 2. Зпустить тесты:
```
 ./CSV_PrettyPrinter --test
```
