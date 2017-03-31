import Exp._ ;


/**

 A reduction/substitution-based interpreter uses program terms as state.

 The core operation is <code>reduce</code>, which transforms an
 expression into an expression using call-by-value beta-reduction.

 */
object SubstitutionInterpreter {

  /**
   A binding-aware substitution replaces all free occurences of a variable with an expression.
   */
  class Substitution(v : Variable, arg : Exp) {

    /**
     Performs capture-avoiding substiution.
     
     It will (silently) corrupt the program if <code>arg</code> contains a free variable.
     */
    def apply(body : Exp) : Exp = body match {
      // [v ==> e']v                             => e'
      case Ref(v_) if v == v_                    => arg

      // [v ==> e']v'                            => v'
      case Ref(_)                                => body
      
      // [v ==> e'](f e)                         => ([v ==> e']f [v ==> e']e)
      case App(f,e)                              => App(this(f),this(e))

      // [v ==> e'](lambda (v) e)                => (lambda (v) e)
      case Lambda(v_,e) if v == v_               => body

      // Guard against capture:
      case Lambda(v_,e) if arg.free contains v_  => throw new Exception("Capture: " + v_ + " would get rebound.")

      // [v ==> e'](lambda (v') e)               => (lambda (v') [v ==> e']e)
      case Lambda(v_,e)                          => Lambda(v_,this(e))
    }
  }

  /**
   Allows a convenient (var ==> term) syntax for substitutions.
   */
  private implicit def stringToSubstitutable(v : Variable) = new Substitutable {
    def ==> (arg : Exp) : Substitution = new Substitution(v,arg)
  }

  private trait Substitutable {
    def ==> (arg : Exp) : Substitution ;
  }

  /**
   Performs a step of call-by-value beta-reduction.
   */
  def reduce (e : Exp) : Exp = e match {

    // References can't be reduced.
    case Ref(_) => e

    // Lambda terms can't be reduced.
    case Lambda(_,_) => e

    // Beta-reduction:
    case App(Lambda(v,body),AtomExp(arg)) => 
      (v ==> arg)(body)

    // Reduce the function term:
    case App(f,AtomExp(e)) => 
      App(reduce(f),e)

    // Reduce the argument term:
    case App(f, e) => 
      App(f,reduce(e))
  }

  /**
   Reduces an expression completely. (If possible.)
   */
  def interpret (exp : Exp) : Exp = {
    val exp_ = reduce(exp)
    if (exp != exp_)
      interpret(exp_)
    else
      exp
  }

  /**
   Traces an execution.
   */
  def trace (exp : Exp) : Exp = {
    println(exp)
    val exp_ = reduce(exp)
    if (exp != exp_)
      trace(exp_)
    else
      exp
  }  


  // This runs to a fixed-point:
  def test1 = 
    trace(ExpParser.parse("((lambda (f) (f f)) (lambda (g) (g g)))"))

  // This would run forever in another interpreter, but stops here
  // when we hit the same term twice:
  def test2 = 
    trace(ExpParser.parse("((lambda (x) x) (lambda (z) z))"))

}
