public class Factorial {
    public static void main(String[] a) {
        System.out.println(new Thing().func());
    }
}

class Thing {
    Element x;// should work
    int a;
    boolean c;
    boolean d;
    int[] e;

    public int func() {
        int[] fung;
        x = new element();// @fail element_ is not a valid Class //fail x_ and expression element_ new are
                          // of different types //fail arooba
        c = new Element();// @fail c_ and expression Element_ new are of different types

        a = 2 * e[3 * 5];

        fung[3] = 0;// succ
        a = e.length;// succ
        a = e[1].length;// @fail e_ is undefined or wrong
        x = e.length;// @fail x_ and expression e_.length are of different types

        x = new int[1];// @fail x_ and expression 1 new are of different types
        fung = new int[true];// @fail true: is of wrong type
        fung = new int[1];

        return x.InInt();// @fail
    }

    public int func2() {
        return x.InInt(1);// should work
    }
}

class Element {
    int a;
    boolean b;
    Element c;

    public int InInt(int x) {
        return a;// should work
    }

    public int InInt2(int x) {
        return x + 1;// should work
    }

    public Element InClass() {
        return c;// should work
    }

    public boolean InBool() {
        return true;// should work
    }

    public boolean InBool2() {
        return b;// should work
    }
}