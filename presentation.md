# MatMul

## 목차

1. 소개
2. 프로젝트 구조
3. 작동 흐름
4. 실행 및 개발 환경 (Makefile)

## 소개

- 이름: MatMul
- 파일 리스트를 읽어서 해당 파일의 내용을 행렬로 가져오고 곱한 결과를 출력하는 프로그램

## 프로젝트 구조

```sh
matmul/
├── bin/        # 실행 파일 (MatMul.exe)
├── include/    # 헤더 파일 (matrix.h, error.h)
├── matrix/     # 결과 확인용 행렬 파일 (A.txt, B.txt)
├── obj/        # 컴파일된 소스 파일 (main.o, matrix.o)
├── src/        # 소스 파일 (main.c, matrix.c)
├── tests/      # 단위 테스트 파일 (test_main.c, test_matrix.c ...)
├── Makefile    # 빌드 및 실행을 위한 Makefile
```

## 작동 흐름 (main)

- 1. argument 체크 (개수)
- 2. 파일 존재 여부 체크
- 3. 각 파일별로 행렬 가져오면서 matrix_multiply 실행
- 4. 결과 행렬 출력
