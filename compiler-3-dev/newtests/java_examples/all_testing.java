class MainClass{
	public static void main(String[] MainString){
    System.out.println(new ArrayTest().Start(10)); //perform array test
    }
 }


class ArrayTest
{
    int[] arrays;
    int startSize;
    int[] array;


    public int Start(int size){
        startSize = 5;
        array = new int[startSize];
        System.out.println(this.Resize(array, size));
        System.out.println(this.Print(array));
        System.out.println(new OperatorTest().Start()); //perform operator test
         return 0;

            }

    public boolean Resize(int[] array, int size){
        boolean didResize;
        int oldSize;
        didResize = false;
        oldSize = array.length;

        if(oldSize < size){
            array = new int[size];
            didResize = true;
        }
        else
            size = size;

        return didResize;
    }

    public int[] Print(int[] array)
    {
        boolean condition;
        int i;
        condition = true;
        i = 0;

        while(i < 10 && condition == true)
        {
            if(i == 9)
            {
                condition = false;
            }else{
                i = i;
            }
            System.out.println(array[i]);
            i = i + 1;
        }
        return array;
    }
}


class OperatorTest extends ArrayTest{

    int sum;

    public int Start(){
        int num1;
        int num2;
        int output;
        num1 = 6;
        num2 = 2;
        output = this.Add(num1, num2);
        output = this.Sub(num1, num2);
        output = this.Mul(num1, num2);
        output = this.Div(num1, num2);
        output = this.Compare(num1, num2);
        return 1;
    }

    public int Add(int num1, int num2){
       sum = num1+num2;
       return sum;
    }
    public int Sub(int num1, int num2){
       if(!(num1 < num2))
           sum = num1-num2;
       else
           sum = num2-num1;

       return sum;
    }
    public int Mul(int num1, int num2){
       sum = num1*num2;
       return sum;
    }
    public int Div(int num1, int num2){
       sum = num1/num2;
       return sum;
    }
    public int Compare(int num1, int num2){
       if(num1 < num2 || num1 > num2){
           sum = num1;
       }else if(num1 == num2 && num2 == num1){
           sum = num2;
       }else{
           sum = 0;
       }
       return sum;
    }
}

// class Orange{
// public Food Fruit(int thing)
// {
// 	Food OrangeFood;
// 	OrangeFood = thing;
// 	return OrangeFood;
// }


