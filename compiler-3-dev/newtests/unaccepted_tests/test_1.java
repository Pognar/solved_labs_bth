// Tests method call being used as a statement
class TestMain1{
    public static void main(String[] a){
        System.out.println(new TestClass1().TestMethod1());
    }
}

class TestClass1{

  public int TestMethod1(){
    int a;
    this.TestMethod1();
    return a;
  }

}
