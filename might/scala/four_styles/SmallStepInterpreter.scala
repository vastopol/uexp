import Exp._ ;



/**
 In a small-step interpreter, each reduction from state-to-state must be computable.

 That is, a small-step interpreter must factor the computation into "computable" chunks.

 To achieve this, a small-step interpreter typically uses contexts and continuations.
 */
object SmallStepInterpreter {
  
  /**
   States are a (partially) evaluated value 
   expression, an environment and a continuation.
   */
  case class State(val valexp : ValExp, val env : Env, val cont : Cont) 

  /**
   Environments map variables to denotable values.
   */
  type Env = Map[Variable,D]

  /**
   A D-object is a (denotable) value.

   A value is denotable if there is a program term which is equivalent
   to it under evaluation.
  */ 
  abstract class D

  /**
   Even though this interpreter has only one kind of value, a closure
   is still seen as an instance of a Val and a Proc.  

   (In case we want to expand the interpreter.)
   */
  abstract class Val extends D
  abstract class Proc extends Val
  case class Clo(lam : Lambda, env : Env) extends Proc

  /**
   Continuations accept a return value to create a new value expression.
   */
  abstract class Cont
  case class RetCont(val context : D => ValExp, val env : Env, val cont : Cont) extends Cont
  case object HaltCont extends Cont

  /**
   A ValExp-object is a partially-evaluated expression.

   That is, some of its sub-expressions have been turned into values.
   */
  abstract class ValExp {
    /**
     Overwrites the leftmost unevaluated expression with this value.
     */
    def insert (d : D) : ValExp ;

    /**
     Returns the leftmost unevaluated expression.
     */
    def nextExp : Exp ;
  }


  /**
   A Den-object represents a sub-expression that has been evaluated.
   */
  case class Den(d : D) extends ValExp {
    def insert (d : D) : ValExp = throw new BadInsertionException()
    def nextExp : Exp = throw new NoNextExpressionsException()
  }

  /**
   A VApp-object represents a partially evaluated application.
   */
  case class VApp(f : ValExp, e : ValExp) extends ValExp {
    def insert (d : D) = {
      try {
        VApp(f.insert(d),e)
      } catch {
        case BadInsertionException() => {
          VApp(f,e.insert(d))
        }
      }
    }
    def nextExp : Exp = {
      try {
        f.nextExp
      } catch {
        case NoNextExpressionsException() => 
          e.nextExp
      }
    }
  }

  /**
   A VExp-object holds expression that cannot be partially evaluated;
   they are either unevaluated or fully evaluated.

   Lambda terms and references are such expressions.
 
   Lambda terms evaluate instantly into closures.

   References evaluate instantly through environment-lookup.
   */
  case class VExp(exp : Exp) extends ValExp {
    def insert (d : D) : ValExp = Den(d)
    def nextExp : Exp = exp    
  }

  /**
   Converts an expression into a ValExp-object in which nothing is
   partially evaluated.
   */
  implicit def expToValExp(exp : Exp) : ValExp = exp match {
    case Ref(v)         => VExp(exp)
    case Lambda(v,body) => VExp(exp)
    case App(f,e)       => VApp(expToValExp(f),expToValExp(e))
  }

  /**
   Returns the next state; this is the deterministic transition relation.
   */
  def next (s : State) : State = s.valexp match {
    
    // Look up a variable:
    case VExp(Ref(v)) =>        
      State(Den(s.env(v)), s.env, s.cont)
    
    // Close over a lambda term:
    case VExp(lam @ Lambda(v,body)) => 
      State(Den(Clo(lam,s.env)), s.env, s.cont)
    
    case VExp(App(_,_)) =>
      throw new Exception("Interpreter error!")
    
    // Apply the function:
    case VApp(Den(Clo(Lambda(v,body),env2)),Den(darg)) =>
      State(body, env2(v) = darg, s.cont)
    
    // Evaluate a sub-expression:
    case VApp(_,_) => 
      State(s.valexp.nextExp, s.env, RetCont(s.valexp.insert(_), s.env, s.cont))
    
    // Return to the current continuation:
    case Den(d) => s.cont match {
      case RetCont(context,env,cont) => State(context(d), env, cont)
      case HaltCont => throw Halted(d)
    }
  }

  /**
   Thrown when trying to insert into an uninsertable value-expressions.
   */
  case class BadInsertionException extends Exception

  /**
   Thrown when trying to get the next expression to evaluate in a value-expression where all sub-terms are evaluated.
   */
  case class NoNextExpressionsException extends Exception

  /**
   Thrown once the machine halts.
   */
  case class Halted(finalValue : D) extends Throwable

  /**
   Continues to take steps until no more steps can be taken.
   */
  def interpret (exp : Exp) : D = {
    try {
      var state = State(exp,Map(),HaltCont)
      while (true) {
        state = next(state)
      }
      throw new Error("There is a bug in the Matrix.")
    } catch {
      case Halted(d) => d
    }
  }
}




