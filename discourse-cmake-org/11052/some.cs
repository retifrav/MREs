using Newtonsoft.Json.Linq;
using System;

public class Program
{
    public static void Main()
    {
        var jsonString = @"{'some':'value','another':1}";
        var obj = JObject.Parse(jsonString);

        Console.WriteLine($"Some: {obj["some"]}");
    }
}
