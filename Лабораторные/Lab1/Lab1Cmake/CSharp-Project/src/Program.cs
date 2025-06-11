using System;

class Program
{
    static void Main(string[] args)
    {
        try
        {
            double x1, x2, x4, x5, x8, x13;
            string x;
            Console.Write("Type X:");
            x = Console.ReadLine();
            if (!double.TryParse(x, out x1))
            {
                Console.WriteLine("Некорректное значение!");
                return;
            }
            x2 = x1 * x1;
            x4 = x2 * x2;
            x5 = x4 * x1;
            x8 = x4 * x4;
            x13 = x5 * x8;
            Console.WriteLine($"x^5 = {x5}; x^13 = {x13}");
            string str = Console.ReadLine();

        }
        catch (Exception ex)
        {
            Console.WriteLine("Ошибка" + ex.Message);
        }
    }
}

