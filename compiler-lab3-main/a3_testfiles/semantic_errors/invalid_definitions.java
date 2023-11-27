public class Factorial {
    public static void main(String[] a) {
        System.out.println(new classtest().aFunc());
    }
}

class classtest {
    int a;
    boolean b;
    int[] c;
    classdata d;

    public int aFunc() {
        e = a; // @fail e_ does not exist in the current scope
        a = d.yFunc();// @fail a_ and expression d_.yFunc_( ) are of different types
        a = d.zFunc();// @fail a_ and expression d_.zFunc_( ) are of different types
        return a;
    }

    public boolean bFunc() {
        b = d.zzFunc(); // @fail zzFunc_ does not exist
        a = d.xFunc();
        b = this.zFunc(); // @fail zFunc_ does not exist
        b = this.aFunc(); // @fail
        b = d.xFunc();// @fail b_ and expression d_.xFunc_( ) are of different types
        b = d.zFunc();// @fail b_ and expression d_.zFunc_( ) are of different types
        return b;
    }

    public int[] cFunc() {
        c = d.xFunc();// @fail c_ and expression d_.xFunc_( ) are of different types
        c = d.yFunc();// @fail c_ and expression d_.yFunc_( ) are of different types
        return c;
    }

    public classdata dFunc() {
        d = d.xFunc();// @fail d_ and expression d_.xFunc_( ) are of different types
        d = d.yFunc();// @fail d_ and expression d_.yFunc_( ) are of different types
        d = d.zFunc();// @fail d_ and expression d_.zFunc_( ) are of different types
        d = d;// accept
        return d;
    }

    public boolean EFUNC() {
        a = b;// @fail a_ and expression b_ are of different types
        a = c;// @fail a_ and expression c_ are of different types
        a = d;// @fail a_ and expression d_ are of different types
        b = a;// @fail b_ and expression a_ are of different types
        b = c;// @fail b_ and expression c_ are of different types
        b = d;// @fail b_ and expression d_ are of different types
        c = a;// @fail c_ and expression a_ are of different types
        c = b;// @fail c_ and expression b_ are of different types
        c = d;// @fail c_ and expression d_ are of different types
        return false;
    }

}

class thatdoExist {
    int a;
}

class classdata {
    int x;
    boolean y;
    int[] z;
    classthatdoesntExist pem;// @fail classthatdoesntExist_ is undefined.
    thatdoExist pen;

    public int xFunc() {
        return x;
    }

    public boolean yFunc() {
        return y;
    }

    public int[] zFunc() {
        return z;
    }
}

class VarUsedBeforeDefinition {
    int a;

    public int b(int b) {
        a = 10; // correct
        b = 10; // correct
        c = 11; // @fail c is not defined yet.
        int c;

        return a;
    }
}