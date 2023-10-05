// Tests what if a method returns a class type?
class TestMain1{
    public static void main(String[] a){
        System.out.println(new TestClass1().TestMethod1());
    }
}

class TestClass1 extends Tree{
    Tree tree;
    public int TestMethod1(){
        Tree treeVar2;
        treeVar2 = tree.TreeMethod();
        return 1;
    }

}

class Tree{
    public Tree TreeMethod(){
        Tree treeVar1; 
        return treeVar1;
    }
}