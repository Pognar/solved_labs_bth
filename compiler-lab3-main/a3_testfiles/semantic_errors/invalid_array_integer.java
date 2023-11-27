public class invalid_array_integer {
    public static void main(String[] a) {
        System.out.println(new A().a2().length);
    }
}

class A {

    public A a1(int num) {
        int[] num_aux;
        num_aux[false] = 2; // @fail invalid type of array index

        if (num_aux[this.a2()] < 1) // @fail invalid type of array index
            num_aux[0] = 1;
        else
            num_aux = this.a2();

        num_aux[3] = num.length; // @fail member .length is used incorrectly
        return this;
    }

    public int[] a2() {
        int a;
        a[0] = 2; // @fail trying to use int as int array
        return a; // @fail type mismatch
    }
}
