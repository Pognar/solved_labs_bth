// Tests: 
// 1. If statements without declaring else. 
// 2. variables of different types in arithmetic operator. 
// 3. What happens if variables are used with logical operators in if statement.
class TestMain7{
    public static void main(String[] a){
        System.out.println(new TestClass7().TestMethod7());
    }
}

class TestClass7 {

  public int TestMethod7(){
      int integer; 
      boolean bool; 
      integer = bool*bool; 
      if(integer && bool)
        integer = 1;
      else
        integer = 2;
      return integer;
  }
}
