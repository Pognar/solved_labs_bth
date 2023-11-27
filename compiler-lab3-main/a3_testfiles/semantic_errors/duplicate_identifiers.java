public class Factorial {
    public static void main(String[] a) {
        System.out.println(new Pemis().Pen(1, 2));

    }
}

class Factorial {// @fail Already Declared Class: Factorial_
    int a;
    int a;// @fail Already Declared variable: a_

    public boolean func() {
        return true;
    }

    public int func() {// @fail Already Declared Function: func_
        return 1;
    }
}

class Pemis {
    boolean b;
    boolean b;// @fail Already Declared variable: b_

    public int Pen(int param, int param) {// @fail Already Declared parameter: param_
        return 1;
    }

    public int Pen2(int param) {
        int param; // @fail Already Declared parameter: param_
        return 1;
    }
}