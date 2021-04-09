import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.ParseCancellationException;
import java.io.*;
import Arithmetic.ArithmeticLexer;
import Arithmetic.ArithmeticParser;

public class Main {
    public static void main(String[] args) {

        // TODO: Imprimir "instruções" de uso...

        boolean flag = true;

        do{
            try {
                System.out.println("Insira uma expressão aritmetica válida:");
                var reader = new BufferedReader(new InputStreamReader(System.in));
                var lexer = new ArithmeticLexer(CharStreams.fromString(reader.readLine()));
                var parser = new ArithmeticParser(new CommonTokenStream(lexer));

                parser.removeErrorListeners();
                parser.setErrorHandler(new BailErrorStrategy());

                Expression expr = getExpression(parser.expression());

                // Verificar se os números são realmente 32 bits
                int fOp = Integer.parseInt(expr.firstOperand);
                int s0p = Integer.parseInt(expr.secondOperand);
                
                // Faz o acesso ao driver e passa a expressão
                var myCalcWriter = new BufferedWriter(new FileWriter("/dev/mycalc"));

                myCalcWriter.write(expr.toString());

                // Pra testar a concorrencia da pra pedir uma confirmação aqui,
                // tipo pedir pra pressionar enter
                // e dps abrir outra instancia do programa.

                myCalcWriter.close();

                var myCalcReader = new BufferedReader(new FileReader("/dev/mycalc"));

                String result = myCalcReader.readLine();

                myCalcReader.close();

                System.out.println("Este é o resultado: " + result);

                flag = false;
            } catch (ParseCancellationException e){
                System.out.println("Incorrect input!");
            } catch (NumberFormatException e){
                System.out.println("Wrong input format");
            } catch (Exception e){
                e.printStackTrace();
                System.out.println("Failed to read input!");
                break;
            }
        }while(flag);
    }

    private static Expression getExpression (ArithmeticParser.ExpressionContext expr) {
        var numberExpressionList = expr.numberExpression();

        String fOp, sOp, op;

        fOp = expr.numberExpression(0).getText();

        op = expr.operation().getText();

        if(expr.NUMBER() != null)
            sOp = expr.NUMBER().getText();
        else
            sOp = numberExpressionList.get(1).getText();

        return new Expression(fOp,sOp, op);
    }
}

class Expression{
    public final String firstOperand, secondOperand, operation;

    Expression(String firstOperand, String secondOperand, String operation) {
        this.firstOperand = firstOperand;
        this.secondOperand = secondOperand;
        this.operation = operation;
    }

    @Override
    public String toString() {
        return firstOperand
                + operation
                + secondOperand;
    }
}
