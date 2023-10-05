// Tests methodcall inside a if condition expression?
class TestMain2{
    public static void main(String[] a){
        System.out.println(new TestClass1().TestMethod1());
    }
}

class TestClass1{
    public int TestMethod1(){
        int a;
        if(this.TestMethod1() == 1){
            a = a+a;
        }else{
            a = a+a;
        }
        return a;
    }
}
