// Tests the use of undefined Types
class TestMain5{
    public static void main(String[] a){
        System.out.println(new TestClass5().TestMethod5());
    }
}

class TestClass5{
  Tree undefinedTree;

  public Leaf TestMethod5(){
    Leaf undefinedLeaf;
    return undefinedLeaf;
  }

}
