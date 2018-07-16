using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class CoffeeMaker
{
    public bool onState;

    //객체 내의 메소드
    public void StartCoffeMaker()
    {
        if (onState == true)
            Console.WriteLine("The CoffeeMaker is already On.");
        else
        {
            onState = true;
            Console.WriteLine("The CoffeeMaker is now on.");
        }
    }
}

class CoffeeMakerApp
{
    public static void Main()
    {
        //new 연산자로 객체를 생성함.
        CoffeeMaker cm = new CoffeeMaker();


        //객체를 통한 메소드 호출
        cm.onState = true;
        cm.StartCoffeMaker();
    }
}


/*
namespace CoffeeMaker
{
    class Program
    {
        static void Main(string[] args)
        {
        }
    }
}
*/


