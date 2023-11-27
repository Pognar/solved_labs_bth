public class Factorial {
    public static void main(String[] a) {
        System.out.println(new classdata().xyFunc());
    }
}

class classtest {
    int a;
}

class classdata {
    int x;
    boolean y;
    int[] z;
    classdata xyz;

    public int xyFunc() {// @fail
        return y;
    }

    public int xzFunc() {// @fail
        return z;
    }

    public boolean yxFunc() {// @fail: Method: yxFunc_ and its return are of different types
        return x;
    }

    public boolean yzFunc() { // @fail
        return this.zxFunc();
    }

    public int[] zxFunc() { // @fail
        return x;
    }

    public int[] zyFunc() { // @fail
        return this.yxFunc();
    }

    public int swFunc() { // @fail
        return z[this.yzFunc()];
    }

}