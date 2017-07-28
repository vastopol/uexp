package tools;

public class Ref<T>
{
    Object value;
    public Ref() {value = null;}
    public T deref() {return (T)value;}
    public T clear() {value = null; return (T)value;}
    public T set(T v) {value = v; return (T)value;}
    public T set(Ref<T> v) {value = v.deref(); return (T)value;}
}