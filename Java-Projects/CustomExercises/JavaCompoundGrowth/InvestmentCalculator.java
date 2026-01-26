import java.util.Scanner;

public class InvestmentCalculator {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.print("Enter initial dollar amount: ");
        double principal = scanner.nextDouble();

        System.out.print("Enter number of years: ");
        int years = scanner.nextInt();

        if (years < 0) {
            System.out.println("Years must be positive.");
            return;
        }


        double rate = 0.035;
        double finalAmount = principal * Math.pow(1 + rate, years);

        System.out.printf("Final amount after %d years: $%.2f%n", years, finalAmount); //Using Printf. Format placeholders and not string concatenation.


        scanner.close();
    }
}

