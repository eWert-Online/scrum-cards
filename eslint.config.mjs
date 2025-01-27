import js from "@eslint/js";
import globals from "globals";
import react from "eslint-plugin-react";
import reactHooks from "eslint-plugin-react-hooks";

import reactRecommended from "eslint-plugin-react/configs/recommended.js";

export default [
  js.configs.recommended,
  {
    ignores: ["dist/*"],
  },
  {
    files: ["**/*.{js,jsx,mjs}"],
    ...reactRecommended,
    settings: {
      react: {
        version: "detect",
      },
    },
    languageOptions: {
      ...reactRecommended.languageOptions,
      ecmaVersion: "latest",
      sourceType: "module",
      globals: {
        ...globals.node,
        ...globals.browser,
      },
    },
    plugins: {
      react: react,
      "react-hooks": reactHooks,
    },
    rules: {
      ...react.configs.flat.recommended.rules,
      ...react.configs.flat["jsx-runtime"].rules,
      ...reactHooks.configs.recommended.rules,
      "no-unused-vars": "off",
      "no-console": "off",
      strict: ["error", "global"],
      curly: ["error", "multi-line"],
      "no-case-declarations": "off",
      "react/no-children-prop": "off",
    },
  },
];
