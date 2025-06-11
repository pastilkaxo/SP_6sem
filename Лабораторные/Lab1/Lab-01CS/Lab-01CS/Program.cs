try
{
    double x1, x2, x4, x5, x8, x13;
    string x;
    string str;
    Console.Write("Type X:");
    x = Console.ReadLine();
    if (!double.TryParse(x, out x1) )
    {
        Console.WriteLine("Bad value!");
         str = Console.ReadLine();
        return;
    }


    if (x1 == 1)
    {
        Console.WriteLine("Bad value! x = 1");
        str = Console.ReadLine();
        return;
    }

    x2 = x1 * x1;
    x4 = x2 * x2;
    x5 = x4 * x1;
    x8 = x4 * x4;
    x13 = x5 * x8;
    Console.WriteLine($"x^5 = {x5}; x^13 = {x13}");
     str = Console.ReadLine();

}
catch(Exception ex)
{
    Console.WriteLine("Error" + ex.Message);
}