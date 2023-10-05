// Tests .Length being used on Integers and Booleans
class TestMain6{
    public static void main(String[] a){
        System.out.println(new TestClass6().TestMethod6());
    }
}

class TestClass6{
  public int TestMethod6(){
    int integer;
    boolean bool;
    integer = integer.length; // This should not work
    bool = bool.length;
    return 1;
  }

}
