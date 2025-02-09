import fs from "fs";
import esbuild from "esbuild";
import postcss from "postcss";
import postcssPresetEnv from "postcss-preset-env";
import stylelint from "stylelint";
import postcssReporter from "postcss-reporter";
import copy from "esbuild-plugin-copy";
import eslint from "esbuild-plugin-eslint";

const isProduction = process.argv[2] === "production";

console.log("Building js bundle...");

esbuild
  .build({
    entryPoints: ["output/src/client/Index.js"],
    outfile: "public/dist/bundle.js",
    publicPath: "",
    bundle: true,
    minify: isProduction,
    sourcemap: !isProduction,
    plugins: [
      eslint({
        throwOnError: true,
        throwOnWarning: true,
      }),
      copy({
        resolveFrom: "cwd",
        assets: [
          {
            from: ["./output/src/client/public/**/*"],
            to: ["./public"],
          },
        ],
      }),
      {
        name: "postcss-plugin",
        setup: (build) => {
          build.onLoad(
            {
              filter: /\.css$/,
            },
            async (args) => {
              const sourceCSS = fs.readFileSync(args.path, {
                encoding: "utf-8",
              });
              const { css } = await postcss([
                stylelint(),
                postcssPresetEnv({
                  stage: 2,
                  features: {
                    "nesting-rules": true,
                    "oklab-function": { preserve: true },
                  },
                }),
                postcssReporter({ clearReportedMessages: true }),
              ]).process(sourceCSS, { from: args.path });

              return {
                contents: css,
                loader: "css",
              };
            }
          );
        },
      },
    ],
    define: {
      "process.env.NODE_ENV": isProduction ? '"production"' : '"development"',
    },
    loader: {
      ".png": "file",
      ".jpg": "file",
      ".gif": "file",
      ".svg": "file",
      ".ttf": "file",
      ".otf": "file",
      ".eot": "file",
      ".woff": "file",
      ".woff2": "file",
    },
  })
  .catch((e) => {
    console.error(e);
    process.exit(1);
  });
