import {ExternalTokenizer} from "@lezer/lr"
import {predefined} from "./parser.terms.js"

const R = 82, L = 76, u = 117, U = 85,
      a = 97, z = 122, A = 65, Z = 90,
      Zero = 48,
      Quote = 34,
      ParenL = 40, ParenR = 41,
      Space = 32, Newline = 10,
      GreaterThan = 62

export const fallback = new ExternalTokenizer(input => {
  if (input.next == GreaterThan) {
    if (input.peek(1) == GreaterThan)
      input.acceptToken(59, 1)
  } else {
    let sawLetter = false, i = 0
    for (;; i++) {
      if (input.next >= A && input.next <= Z) sawLetter = true
      else if (input.next >= a && input.next <= z) return
      else if (input.next != 95 && !(input.next >= Zero && input.next <= Zero + 9)) break
      input.advance()
    }
    if (sawLetter && i > 1) input.acceptToken(predefined)
  }
}, {extend: true})
