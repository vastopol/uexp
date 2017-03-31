import Exp._ ;



/** 

 A big-step interpreter is an "operational" interpreter that reduces one
 program state into another.

 A big-step interpreter is allowed to use a (potentially) non-terminating reduction.

 */
object BigStepInterpreter {
  
  /** 
   A state is an expression closure.
   */
  type State = ExpClo

  /**
   Big-step environments map variables to <i>expression</i> closures.
   */
  type Env = Map[Variable,ExpClo] 

  /**
   An expression closure is an expression paired with an environment
   to determine the values of its free variables.
   */
  case class ExpClo(val exp : Exp, val env : Env) {

    /**
     Fully reduces an expression closure, or else it non-terminates.
     */
    def reduced : ExpClo = exp match {

      // Closures over lambda terms are final.
      case Lambda(_,_) => this

      case Ref(v) => env(v) // Already reduced in call-by-value.

      case App(f,e) => {
        val fClo = ExpClo(f,env).reduced
        val eClo = ExpClo(e,env).reduced
        val ans = fClo match {
          case ExpClo(Lambda(v,body),env2) => ExpClo(body,env2(v) = eClo)
          case _ => throw new Exception("error: applying non-procedure: " + fClo)
        }
        
        ans.reduced
      }
    }
  }



  /**
   Reduces an expression to its final state.
   */
  def interpret (exp : Exp) : State = 
    ExpClo(exp,Map()).reduced
}

