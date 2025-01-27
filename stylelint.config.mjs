export default {
  plugins: ["stylelint-use-logical"],
  extends: ["stylelint-config-order-66/error"],
  rules: {
    "csstools/use-logical": true,
    // "csstools/value-no-unknown-custom-properties": [
    //   true,
    //   {
    //     importFrom: [
    //       "src/client/css/colors.css",
    //       "src/client/css/definitions.css",
    //     ],
    //   },
    // ],
    "rule-empty-line-before": [
      "always",
      {
        except: ["first-nested"],
        ignore: ["after-comment"],
      },
    ],
    "block-no-empty": true,
    "font-weight-notation": "numeric",
    "color-hex-length": "long",
    "color-no-invalid-hex": true,
    "comment-no-empty": true,
    "declaration-block-no-duplicate-properties": true,
    "declaration-block-no-shorthand-property-overrides": true,
    "declaration-block-single-line-max-declarations": 1,
    "function-url-quotes": "always",
    "media-feature-name-no-vendor-prefix": true,
    "length-zero-no-unit": true,
    "property-no-vendor-prefix": true,
    "selector-no-vendor-prefix": true,
    "selector-pseudo-element-colon-notation": "double",
    "value-no-vendor-prefix": true,
    "color-function-notation": "modern",
    "font-family-no-duplicate-names": true,
    "font-family-no-missing-generic-family-keyword": true,
    "function-calc-no-unspaced-operator": true,
    "function-linear-gradient-no-nonstandard-direction": true,
    "keyframe-declaration-no-important": true,
    "media-feature-name-no-unknown": true,
    "no-duplicate-at-import-rules": true,
    "no-duplicate-selectors": true,
    "no-empty-source": true,
    "property-no-unknown": true,
    "selector-pseudo-class-no-unknown": true,
    "selector-pseudo-element-no-unknown": true,
    "selector-type-no-unknown": true,
    // "selector-class-pattern": [
    //   "^(is-(?:[a-z]+(?:[A-Z][a-z]+)*)|(u-(?:[a-z]+(?:[A-Z][a-z]+)*))|((?:[A-Z][a-z]+(?:[A-Z][a-z]+)*))(--?(?:[a-z]+(?:[A-Z][a-z]+)*))?)$",
    //   {
    //     message: (pattern) =>
    //       `"${pattern}": Expected selectors to follow the scheme defined in the coding guidelines. (<ComponentName>[-descendentName][--modifierName] || u-<utilityName> || is-<stateName>)`,
    //   },
    // ],
    // "custom-property-pattern": [
    //   "^([A-Z][a-z]+(?:[A-Z][a-z]+)*)-(spacing|color|gradient|textDecoration|opacity|radius|size|fontSize)-((?:[a-z]|[0-9])+(?:[A-Z]|[a-z]+|[0-9]*)*)$",
    //   {
    //     message: (pattern) =>
    //       `"${pattern}": Expected variable names to follow the scheme defined in the coding guidelines. (Variables should start with the component name in which they are used, followed by the type of variable and ending with a descriptive name. Each segment is separated by a hyphen.)`,
    //   },
    // ],
    "string-no-newline": true,
    "unit-no-unknown": true,
    "at-rule-no-unknown": [
      true,
      {
        ignoreAtRules: ["mixin", "include", "if", "else"],
      },
    ],
    // "margin" and "padding" shorthands do not use logical values currently:
    // https://github.com/w3c/csswg-drafts/issues/1282
    "property-disallowed-list": [
      [
        "border-color",
        "border-style",
        "border-width",
        "inset",
        "margin",
        "padding",
        "scroll-margin",
        "scroll-padding",
      ],
      {
        message: (prop) =>
          `Use longhand form of ${prop} (ex. "margin-block", "margin-inline"), so that logical properties will be used`,
        severity: "warning",
      },
    ],
    "order/order": [
      [
        { type: "at-rule", name: "import" },
        { type: "at-rule", name: "forward" },
        { type: "at-rule", name: "use" },
        "dollar-variables",
        "at-variables",
        "custom-properties",
        { type: "at-rule", name: "custom-media" },
        { type: "at-rule", name: "function" },
        { type: "at-rule", name: "mixin" },
        { type: "at-rule", name: "extend" },
        "declarations",
        { type: "at-rule", name: "media", hasBlock: true },
        {
          type: "rule",
          selector: /^&::[\w-]+/,
          hasBlock: true,
        },
        "rules",
        { type: "at-rule", name: "media", hasBlock: true },
      ],
      {
        severity: "warning",
      },
    ],
  },
};
