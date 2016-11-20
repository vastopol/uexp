% configuration.fp - default configuration

configuration = {
  cflags = ~"-I."
  prelude = ~"prelude.fp"
  cc = ~"gcc"
  limit = ~5
  entrypoint = ~`_start`
}
