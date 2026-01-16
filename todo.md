# Matrix multiplication

# 목표

- 다음과 같이 행렬곱을 수행하는 프로그램 MatMul.exe 를 구현하세요.

```powershell
> MatMul.exe A.txt B.txt
> A =
1 2
4 5
B =
1 0
0 1
AB =
1 2
4 5
```

# 행렬 파일의 서식

```
#row #col
m11 m12 m13
m21 m22 m23
```

- 첫번째 줄은 행렬의 행 갯수와 열 갯수를 의미합니다.
- 두번째 줄부터는 행렬의 원소들이 적혀있습니다.
- 모든 숫자 사이는 공백으로 구분됩니다.
- 예는 다음과 같습니다.

```
2 3
1 2 3
4 5 6
```

```
3 2
1 4
2 5
3 6
```

- desired output

```
14 32
32 77
```

# 조건

- C/C++ 로 구현하세요.
- 프로그램에 입력되는 행렬 파일의 서식에는 오류가 없다고 가정합니다.
- 여러 개의 행렬을 입력으로 받으면 순서대로 곱하여 결과를 낼 수 있어야합니다.

# pseudocode

- argument dectection
- open the file and check is exist. (validation)
- if exist, read the file and parse to matrix
- print each matrix with the filename as name of matrix
- matrix validation
  - check is matmul available (mxn, nxp) for each mult
- do matmul + combine each name
- print the output

# Note

- use dynamic array for matrix
  - fixed size: memory leak
- how to make unit test on c?
  - use Unity (unit test framework)
