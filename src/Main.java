import org.antlr.v4.runtime.*;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import Arithmetic.ArithmeticLexer;
import Arithmetic.ArithmeticParser;
import org.antlr.v4.runtime.misc.ParseCancellationException;

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

                // Código de teste
                var expressionContext = parser.expression();

                var expr = getExpression(expressionContext);
                System.out.println(expr);

                // TODO: Verificar se os números são realmente 32 bits...
                // TODO: Fazer o acesso ao driver e passar a expressão...

                // Pra testar a concorrencia da pra pedir uma confirmação aqui,
                // tipo pedir pra pressionar enter
                // e dps abrir outra instancia do programa.

                // A alteração dessa flag deve ser sempre a ultima feita
                flag = false;
            } catch (ParseCancellationException e){
                System.out.println("Incorrect input!");
            } catch (Exception e){
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
