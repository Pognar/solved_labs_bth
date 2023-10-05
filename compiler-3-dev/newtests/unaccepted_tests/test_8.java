// Tests what if boolean is used for position in an array?
class TestMain8{
    public static void main(String[] a){
        System.out.println(new TestClass8().TestMethod8());
    }
}

class TestClass8{

  public int TestMethod8(boolean yes){
    int[] array; 
    boolean bool; 
    int integer; 
    integer = this.test(1+2, true).TestMethod8(2);
    //integer = array[bool];
    return integer;
  }

  public TestClass8 test(int a, boolean b){

    return this;
  }
}

