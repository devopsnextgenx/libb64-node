const b64 = require('./build/Release/b64');
// addon.hello();

try {
  const result = b64.sum(5, 7);
  console.log('Sum is:', result);
  console.log(b64.helloWorld("Amit"));
} catch (e) {
  console.error('Error:', e.message);
}


// Encoding
const encoded = b64.encode("AmoghKshirsagar");
console.log(encoded); // SGVsbG8sIFdvcmxkIQ==

// Decoding
const decoded = b64.decode(encoded);
console.log(decoded); // Hello, World!