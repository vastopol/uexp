import scala.util.parsing.combinator._ ;


/**
 An abstract syntax tree for a toy lambda-calculus-like language.
 */

object Exp {
  type Variable = String
}

import Exp._

/**
 A grammar for expressions:

 f,e in Exp ::= v | lam | app

   v in Var is  a set of identifiers

 lam in Lam ::= (lambda (<v>) <e>)

 app in App ::= (<f> <e>)

 */


/**
 An abstract syntax tree node representing an expression.
 */
sealed abstract class Exp {
  /**
   The free variables of the expression.
   */
  def free : Set[Variable]
}


/**
 An abstract syntax tree node representing a variable reference.
 */
case class Ref(v : Variable) extends Exp {
  lazy val free = Set(v)

  override def equals (a : Any) = a match {
    case Ref(v_) => v == v_
    case _ => false
  }
  
  override def toString = v
}


/**
 An abstract syntax tree node representing a lambda term:
 <code>(lambda (v) e)</code>.
 */
case class Lambda(v : Variable, body : Exp) extends Exp {
  lazy val free = body.free - v

  override def equals (a : Any) = a match {
    case Lambda(v_,body_) => (v == v_) && (body == body_)
    case _ => false
  }

  override def toString = "(lambda ("+v+") "+body+")"
}


/**
 An abstract syntax tree node representing an application: 
 <code>(f e)</code>.
 */
case class App(f : Exp, arg : Exp) extends Exp {
  lazy val free = f.free ++ arg.free

  override def equals (a : Any) = a match {
    case App(f_,arg_) => (f == f_) && (arg == arg_)
    case _ => false
  }

  override def toString = "("+f+" "+arg+")"
}


/**
 Atomic expressions can be evaluated without side-effects or non-termination.

 References and lambda-expressions are atomic.
 */
object AtomExp {
  def unapply(e : Exp) : Option[Exp] = e match {
    case Ref(_) | Lambda(_,_) => Some(e)
    case _ => None
  }
}



/**
 A parser for a toy lambda-calculus-like language.
 */
object ExpParser extends JavaTokenParsers {

  def expr : Parser[Exp] = ref | lambda | app

  def lambda : Parser[Exp] = "(" ~ "lambda" ~ "(" ~ ident ~ ")" ~ expr ~ ")" ^^
         { case "(" ~ "lambda" ~ "(" ~ id ~ ")" ~ e ~ ")" => Lambda(id,e) }

  def app : Parser[Exp] = "(" ~ expr ~ expr ~ ")" ^^
         { case "(" ~ f ~ a ~ ")" => App(f,a) }

  def ref : Parser[Exp] = ident ^^
         { case id => Ref(id) }

  def parse(reader : java.io.Reader) : Exp = parseAll(expr,reader).get

  def parse(s : String) : Exp = parse(new java.io.StringReader(s))
}

