// Tests:
//1. Multiple classes with same name.
// 2. Multiple methods with same name.
// 3. Multiple variables with same name.
class TestMain3{
    public static void main(String[] a){
        System.out.println(new TestClass3().TestMethod3());
    }
}

class TestClass3{
  int[] array1 ;
  //int[] array1 ;


  public int TestMethod3(){
    int array1 ;
    array1 = array1[0];
    return array1;
  }

  // public int TestMethod3(){
  //   int b ;
  //   return b;
  // }
}

// class TestClass3{
//   int[] array1 ;


//   public int TestMethod3(){
//     int c ;
//     return c;
//   }
// }
