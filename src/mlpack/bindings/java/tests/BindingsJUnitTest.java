package org.mlpack;

import static org.junit.Assert.*;

import org.junit.Test;
import org.mlpack.BindingsTest;
import org.nd4j.linalg.factory.Nd4j;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.nd4j.linalg.api.buffer.DataType;

import java.util.*;

public class BindingsJUnitTest {
  @Test
  public void runCorrectly() {
    BindingsTest.Params params = new BindingsTest.Params();
    params.put("int_in", 12);
    params.put("double_in", 4.0);
    params.put("string_in", "hello");
    params.put("flag1", true);

    BindingsTest.run(params);

    assertEquals("hello2", params.get("string_out", String.class));
    assertEquals(13, (int) params.get("int_out", Integer.class));
    assertEquals(5.0, (double) params.get("double_out", Double.class), 0.0001);
  }

  @Test
  public void runForgotFlag() {
    BindingsTest.Params params = new BindingsTest.Params();
    params.put("int_in", 12);
    params.put("double_in", 4.0);
    params.put("string_in", "hello");

    BindingsTest.run(params);

    assertNotEquals("hello2", params.get("string_out", String.class));
    assertNotEquals(13, (int) params.get("int_out", Integer.class));
    assertNotEquals(5.0, (double) params.get("double_out", Double.class), 0.0001);
  }

  @Test
  public void runWithWrongString() {
    BindingsTest.Params params = new BindingsTest.Params();
    params.put("int_in", 12);
    params.put("double_in", 4.0);
    params.put("string_in", "goodbye");
    params.put("flag1", true);

    BindingsTest.run(params);

    assertNotEquals("hello2", params.get("string_out", String.class));
  }

  @Test
  public void runWithWrongInt() {
    BindingsTest.Params params = new BindingsTest.Params();
    params.put("int_in", 15);
    params.put("double_in", 4.0);
    params.put("string_in", "hello");
    params.put("flag1", true);

    BindingsTest.run(params);

    assertNotEquals(13, (int) params.get("int_out", Integer.class));
  }

  @Test
  public void runWithWrongDouble() {
    BindingsTest.Params params = new BindingsTest.Params();
    params.put("int_in", 12);
    params.put("double_in", 10.0);
    params.put("string_in", "hello");
    params.put("flag1", true);

    BindingsTest.run(params);

    assertNotEquals(5.0, params.get("double_out", Double.class), 0.0001);
  }

  @Test
  public void runWithWrongFlag() {
    BindingsTest.Params params = new BindingsTest.Params();
    params.put("int_in", 12);
    params.put("double_in", 4.0);
    params.put("string_in", "hello");
    params.put("flag1", true);
    params.put("flag2", true);

    BindingsTest.run(params);

    assertNotEquals("hello2", params.get("string_out", String.class));
    assertNotEquals(13, (int) params.get("int_out", Integer.class));
    assertNotEquals(5.0, (double) params.get("double_out", Double.class), 0.0001);
  }

  @Test
  public void runWithMatrix() {
    INDArray m = Nd4j.rand(DataType.DOUBLE, 'c', new long[] {100, 5});

    BindingsTest.Params params = new BindingsTest.Params();
    params.put("int_in", 12);
    params.put("double_in", 4.0);
    params.put("string_in", "hello");
    params.put("flag1", true);
    params.put("matrix_in", m);

    BindingsTest.run(params);

    INDArray result = params.get("matrix_out", INDArray.class);
    assertEquals(100, result.size(0));
    assertEquals(4, result.size(1));
    assertEquals(DataType.DOUBLE, result.dataType());

    int[] rows = {0, 1, 3};
    for (int i : rows) {
      for (int j = 0, n = (int) result.size(1); j < n; ++j) {
        assertEquals(m.getDouble(j, i), result.getDouble(j, i), 0.0001);
      }
    }

    for (int i = 0, n = (int) result.size(1); i < n; ++i) {
      assertEquals(m.getDouble(i ,2) * 2, result.getDouble(i, 2), 0.0001);
    }
  }

  @Test
  public void runWithMatrixColMajor() {
    INDArray m = Nd4j.rand(DataType.DOUBLE, 'f', new long[] {100, 5});

    BindingsTest.Params params = new BindingsTest.Params();
    params.put("int_in", 12);
    params.put("double_in", 4.0);
    params.put("string_in", "hello");
    params.put("flag1", true);
    params.put("matrix_in", m);

    BindingsTest.run(params);

    INDArray result = params.get("matrix_out", INDArray.class);
    assertEquals(100, result.size(0));
    assertEquals(4, result.size(1));
    assertEquals(DataType.DOUBLE, result.dataType());


    int[] rows = {0, 1, 3};
    for (int i : rows) {
      for (int j = 0, n = (int) result.size(1); j < n; ++j) {
        assertEquals(m.getDouble(j, i), result.getDouble(j, i), 0.0001);
      }
    }

    for (int i = 0, n = (int) result.size(1); i < n; ++i) {
      assertEquals(m.getDouble(i ,2) * 2, result.getDouble(i, 2), 0.0001);
    }
  }

  @Test
  public void runWithUMatrix() {
    int[] data = {
      1, 2, 3, 4, 5,
      6, 7, 8, 9, 10,
      11, 12, 13, 14, 15,
      16, 17, 18, 19, 20,
      21, 22, 23, 24, 25
    };

    long[] shape = {5, 5};
    INDArray m = Nd4j.create(data, shape, DataType.UINT64);

    BindingsTest.Params params = new BindingsTest.Params();
    params.put("int_in", 12);
    params.put("double_in", 4.0);
    params.put("string_in", "hello");
    params.put("flag1", true);
    params.put("umatrix_in", m);

    BindingsTest.run(params);

    INDArray result = params.get("umatrix_out", INDArray.class);
    assertEquals(5, result.size(0));
    assertEquals(4, result.size(1));
    assertEquals(DataType.UINT64, result.dataType());

    int[] rows = {0, 1, 3};
    for (int i : rows) {
      for (int j = 0, n = (int) result.size(1); j < n; ++j) {
        assertEquals(m.getLong(j, i), result.getLong(j, i));
      }
    }

    for (int i = 0, n = (int) result.size(1); i < n; ++i) {
      assertEquals(m.getLong(i, 2) * 2, result.getLong(i, 2));
    }
  }

  @Test
  public void runWithUMatrixColMajor() {
    int[] data = {
      1, 2, 3, 4, 5,
      6, 7, 8, 9, 10,
      11, 12, 13, 14, 15,
      16, 17, 18, 19, 20,
      21, 22, 23, 24, 25
    };

    long[] shape = {5, 5};
    char order = 'f';
    long[] strides = Nd4j.getStrides(shape, order);
    INDArray m = Nd4j.create(data, shape, strides, order, DataType.UINT64);

    BindingsTest.Params params = new BindingsTest.Params();
    params.put("int_in", 12);
    params.put("double_in", 4.0);
    params.put("string_in", "hello");
    params.put("flag1", true);
    params.put("umatrix_in", m);

    BindingsTest.run(params);

    INDArray result = params.get("umatrix_out", INDArray.class);
    assertEquals(m.size(0), result.size(0));
    assertEquals(m.size(1) - 1, result.size(1));
    assertEquals(DataType.UINT64, result.dataType());

    int[] rows = {0, 1, 3};
    for (int i : rows) {
      for (int j = 0, n = (int) result.size(1); j < n; ++j) {
        assertEquals(m.getLong(j, i), result.getLong(j, i));
      }
    }

    for (int i = 0, n = (int) result.size(1); i < n; ++i) {
      assertEquals(m.getLong(i, 2) * 2, result.getLong(i, 2));
    }
  }

  @Test
  public void runWithCol() {
    INDArray data = Nd4j.rand(DataType.DOUBLE, 'c', new long[] {100});

    BindingsTest.Params params = new BindingsTest.Params();
    params.put("int_in", 12);
    params.put("double_in", 4.0);
    params.put("string_in", "hello");
    params.put("flag1", true);
    params.put("col_in", data);

    BindingsTest.run(params);

    INDArray result = params.get("col_out", INDArray.class);
    assertEquals(data.size(0), result.size(0));
    assertEquals(DataType.DOUBLE, result.dataType());

    for (int i = 0, n = (int) data.size(0); i < n; ++i) {
      assertEquals(data.getDouble(i) * 2, result.getDouble(i), 0.0001);
    }
  }

  @Test
  public void runWithUCol() {
    long[] raw = {3, 4, 5, 6, 7, 8, 9, 2, 4, 4};

    INDArray data = Nd4j.create(raw, new long[] {raw.length}, DataType.UINT64);

    BindingsTest.Params params = new BindingsTest.Params();
    params.put("int_in", 12);
    params.put("double_in", 4.0);
    params.put("string_in", "hello");
    params.put("flag1", true);
    params.put("ucol_in", data);

    BindingsTest.run(params);

    INDArray result = params.get("ucol_out", INDArray.class);
    assertEquals(data.size(0), result.size(0));
    assertEquals(DataType.UINT64, result.dataType());

    for (int i = 0, n = (int) result.size(0); i < n; ++i) {
      assertEquals(data.getLong(i) * 2, result.getLong(i));
    }
  }

  @Test
  public void runWithRow() {
    INDArray data = Nd4j.rand(DataType.DOUBLE, 'c', new long[] {100});

    BindingsTest.Params params = new BindingsTest.Params();
    params.put("int_in", 12);
    params.put("double_in", 4.0);
    params.put("string_in", "hello");
    params.put("flag1", true);
    params.put("row_in", data);

    BindingsTest.run(params);

    INDArray result = params.get("row_out", INDArray.class);
    assertEquals(data.size(0), result.size(0));
    assertEquals(DataType.DOUBLE, result.dataType());

    for (int i = 0, n = (int) result.size(0); i < n; ++i) {
      assertEquals(data.getDouble(i) * 2, result.getDouble(i), 0.0001);
    }
  }

  @Test
  public void runWithURow() {
    long[] raw = {3, 4, 5, 6, 7, 8, 9, 2, 4, 4};

    INDArray data = Nd4j.create(raw, new long[] {raw.length}, DataType.UINT64);

    BindingsTest.Params params = new BindingsTest.Params();
    params.put("int_in", 12);
    params.put("double_in", 4.0);
    params.put("string_in", "hello");
    params.put("flag1", true);
    params.put("urow_in", data);

    BindingsTest.run(params);

    INDArray result = params.get("urow_out", INDArray.class);
    assertEquals(data.size(0), result.size(0));
    assertEquals(DataType.UINT64, result.dataType());

    for (int i = 0, n = (int) result.size(0); i < n; ++i) {
      assertEquals(data.getLong(i) * 2, result.getLong(i));
    }
  }

  @Test
  public void runWithIntVector() {
    List<Integer> data = Arrays.asList(1, 2, 3, 4, 5);

    BindingsTest.Params params = new BindingsTest.Params();
    params.put("int_in", 12);
    params.put("double_in", 4.0);
    params.put("string_in", "hello");
    params.put("flag1", true);
    params.put("vector_in", data);

    BindingsTest.run(params);

    List<Integer> result = params.get("vector_out", List.class);
    assertEquals(data.size() - 1, result.size());

    for (int i = 0, n = result.size(); i < n; ++i) {
      assertEquals(data.get(i), result.get(i));
    }
  }

  @Test
  public void runWithStringVector() {
    List<String> data = Arrays.asList("one", "two", "three", "four", "five");

    BindingsTest.Params params = new BindingsTest.Params();
    params.put("int_in", 12);
    params.put("double_in", 4.0);
    params.put("string_in", "hello");
    params.put("flag1", true);
    params.put("str_vector_in", data);

    BindingsTest.run(params);

    List<String> result = params.get("str_vector_out", List.class);
    assertEquals(data.size() - 1, result.size());

    for (int i = 0, n = result.size(); i < n; ++i) {
      assertEquals(data.get(i), result.get(i));
    }
  }

  @Test
  // Supposed to fail but doesn't, need to investigate
  public void runWithSetOrder() {
    int[] arr = new int[25];
    for (int i = 0; i < arr.length; ++i) {
      arr[i] = i;
    }

    long[] shape = {5, 5};
    INDArray matrix = Nd4j.create(arr, shape, DataType.UINT64);

    assertEquals('c', matrix.ordering());

    matrix.setOrder('f');

    assertEquals('f', matrix.ordering());

    BindingsTest.Params params = new BindingsTest.Params();
    params.put("int_in", 12);
    params.put("double_in", 4.0);
    params.put("string_in", "hello");
    params.put("flag1", true);
    params.put("umatrix_order_in", matrix);

    BindingsTest.run(params);

    INDArray result = params.get("umatrix_order_out", INDArray.class);
    assertEquals(matrix, result);
  }

  @Test
  public void doubleDeallocation() {
    int[] arr = new int[100];
    for (int i = 0; i < arr.length; ++i) {
      arr[i] = i;
    }

    long[] shape = {10, 10};
    INDArray matrix = Nd4j.create(arr, shape, DataType.UINT64);

    BindingsTest.Params params = new BindingsTest.Params();
    params.put("int_in", 12);
    params.put("double_in", 4.0);
    params.put("string_in", "hello");
    params.put("flag1", true);
    params.put("umatrix_order_in", matrix);

    BindingsTest.run(params);

    INDArray result = params.get("umatrix_order_out", INDArray.class);
    assertEquals(matrix, result);

    matrix = null;
    result = null;
    params = null;

    // This won't actually cause double deallocation because arma copies memory,
    // when matrix is being moved (true only for [copy_aux_mem=false, strict=true] constructor params)
    System.gc();

    assertTrue(true);
  }

  @Test
  public void runWithModel() {
    BindingsTest.Params params = new BindingsTest.Params();
    params.put("int_in", 12);
    params.put("double_in", 4.0);
    params.put("string_in", "hello");
    params.put("build_model", true);

    BindingsTest.run(params);

    params.put("build_model", false);
    params.put("model_in", params.get("model_out", GaussianKernelType.class));

    BindingsTest.run(params);

    assertEquals(20.0, params.get("model_bw_out", Double.class), 0.0001);
  }

  @Test
  public void runWithMatrixAndInfo() {
    INDArray matrix = Nd4j.rand(DataType.DOUBLE, 'c', new long[] {100, 10});
    boolean[] info = new boolean[100];
    MatrixWithInfo data = new MatrixWithInfo(matrix, info);

    BindingsTest.Params params = new BindingsTest.Params();
    params.put("int_in", 12);
    params.put("double_in", 4.0);
    params.put("string_in", "hello");
    params.put("matrix_and_info_in", data);

    BindingsTest.run(params);

    INDArray result = params.get("matrix_and_info_out", INDArray.class);
    assertEquals(matrix.size(0), result.size(0));
    assertEquals(matrix.size(1), result.size(1));

    for (int i = 0, n = (int) result.size(0); i < n; ++i) {
      for (int j = 0, m = (int) result.size(1); j < m; ++j) {
        assertEquals(matrix.getDouble(i, j) * 2, result.getDouble(i, j), 0.0001);
      }
    }
  }

  @Test(expected = AssertionError.class)
  // this test is almost identical to the next one but will fail
  public void runWithRowViewFail() {
    int[] data = {
      1, 2, 3, 4, 5,
      6, 7, 8, 9, 10,
      11, 12, 13, 14, 15,
      16, 17, 18, 19, 20,
      21, 22, 23, 24, 25
    };

    long[] shape = {5, 5};
    char order = 'f';
    long[] strides = Nd4j.getStrides(shape, order);
    INDArray matrix = Nd4j.create(data, shape, strides, order, DataType.DOUBLE);

    BindingsTest.Params params = new BindingsTest.Params()
        .put("int_in", 12)
        .put("double_in", 4.0)
        .put("string_in", "hello")
        .put("row_view_in", matrix.getRow(0, true));

    BindingsTest.run(params);

    INDArray result = params.get("row_view_out", INDArray.class);
    assertEquals(result, matrix.getRow(0));
  }

  @Test
  public void runWithRowView() {
    int[] data = {
      1, 2, 3, 4, 5,
      6, 7, 8, 9, 10,
      11, 12, 13, 14, 15,
      16, 17, 18, 19, 20,
      21, 22, 23, 24, 25
    };

    long[] shape = {5, 5};
    char order = 'c';
    long[] strides = Nd4j.getStrides(shape, order);
    INDArray matrix = Nd4j.create(data, shape, strides, order, DataType.DOUBLE);

    BindingsTest.Params params = new BindingsTest.Params()
        .put("int_in", 12)
        .put("double_in", 4.0)
        .put("string_in", "hello")
        .put("row_view_in", matrix.getRow(0, true));

    BindingsTest.run(params);

    INDArray result = params.get("row_view_out", INDArray.class);
    assertEquals(matrix.getRow(0), result);
  }

  @Test
  public void runWithColView() {
    int[] data = {
      1, 2, 3, 4, 5,
      6, 7, 8, 9, 10,
      11, 12, 13, 14, 15,
      16, 17, 18, 19, 20,
      21, 22, 23, 24, 25
    };

    long[] shape = {5, 5};
    char order = 'c';
    long[] strides = Nd4j.getStrides(shape, order);
    INDArray matrix = Nd4j.create(data, shape, strides, order, DataType.DOUBLE);

    BindingsTest.Params params = new BindingsTest.Params()
        .put("int_in", 12)
        .put("double_in", 4.0)
        .put("string_in", "hello")
        .put("col_view_in", matrix.getColumn(0, true));

    BindingsTest.run(params);

    INDArray result = params.get("col_view_out", INDArray.class);
    assertEquals(result, matrix.getColumn(0));
  }
}
