//Test undeclared variable, method, class
class TestMain4{
    public static void main(String[] a){
        System.out.println(new TestClass4().TestMethod());
    }
}

class TestClass4 extends UndeclaredClass{

  public int TestMethod(){
    int a;
   //int UndeclaredVariable;
    a = a + UndeclaredVariable;
    a = this.UndeclaredMethod();
    return a;
  }

  // public int UndeclaredMethod(){
  //  int a;
  //  return a;
  // }
}

// class UndeclaredClass{
//  public int TestMethod(){
//    return a;
//  }
// }
