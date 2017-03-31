import Exp._ ;


/**

 A denotational interpreter transforms a program term into a
 corresponding structure in the host language; that corresponding
 structure is the term's "denotation."

 For instance, a lambda term becomes a function, and a numeral would
 become an integer.

 */
object DenotationalInterpreter {


  /**
   D is the domain of values.

   Historically, D has been used to mean "Denotation," "Denotable
   value" and "Domain."
   */
  abstract class D {
    def apply(d : D) : D ;
  }

  /**
   In the pure lambda calculus, the only kind of value is a function.
   */
  case class Fun (f : D => D) extends D {
    def apply(d : D) : D = f(d)
  }

  /**
   Environments map variables to values.
   */
  type Env = Map[Variable,D]

  /**
   Transforms an environment and an expression into a value.
   */
  def eval (env : Env) (e : Exp) : D = e match {
    case Ref(v)      => env(v)
    case App(f,e)    => (eval(env)(f)) (eval(env)(e))
    case Lambda(v,e) => Fun(d => eval(env(v) = d)(e))
  }

  /**
   Converts an expression into a value.
   */
  def interpret (exp : Exp) : D = eval (Map()) (exp)
}



