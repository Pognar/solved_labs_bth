public class Factorial {
    public static void main(String[] a) {
        System.out.println(new classtest().semantically_incorrect_expressions());
    }
}

class classtest {
    int i1;
    int i2;
    boolean b1;
    boolean b2;
    int[] ia1;
    int[] ia2;
    ClassDatatest c1;
    ClassDatatest c2;

    public boolean semantically_incorrect_expressions() {
        i1 = i1 + b1;// @fail b1_: is of wrong type
        i1 = i1 + ia1;// @fail ia1_: is of wrong type
        i1 = i1 + c1;// @fail c1_: is of wrong type
        i1 = b1 + b1;// @fail b1_: is of wrong type
        i1 = ia1 + ia1;// @fail ia1_: is of wrong type
        i1 = ia1 + ia1[0];// @fail ia1_: is of wrong type
        i1 = c1 + c1;// @fail c1_: is of wrong type
        i1 = c1 && c1;// @fail c1_: is of wrong type //fail i1_ and expression c1_ && c1_ are of
                      // different types
        i1 = c1 || c1;// @fail
        i1 = c1 < c1;// @fail
        b1 = b1 + b1;// @fail
        b1 = b1 + ia1;// @fail
        b1 = b1 && ia1;// @fail
        b1 = b1 == c1;// @fail
        ia1 = ia1 + ia2;// @fail
        ia1 = ia1 && ia2;// @fail
        b1 = b1 < b2;// @fail
        b1 = !i1;// @fail
        return true;
    }

    // works
    public boolean operator_test_work() {
        i1 = i1 + i2;
        i1 = ia1[1] + ia2[2];
        i1 = c1.get_trash()[2] + i2;
        b1 = b1 && b2;
        b1 = i1 < i2;
        return true;
    }

}

class ClassDatatest {
    int[] trash;

    public int[] get_trash() {
        return trash;
    }
}
