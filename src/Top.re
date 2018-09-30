module StringSet = Set.Make(String);

type action =
  | DataLoaded(Js.Dict.t(array(string)))
  | TopWordsLoaded(StringSet.t)
  | ChangeQuery(string);

type state = {
  thesaurus: option(Js.Dict.t(array(string))),
  topwords: option(StringSet.t),
  query: string,
};

let component = ReasonReact.reducerComponent("Top");

let make = _children => {
  ...component,
  initialState: () => {query: "", topwords: None, thesaurus: None},
  reducer: (action, state) =>
    switch (action) {
    | DataLoaded(data) =>
      ReasonReact.Update({...state, thesaurus: Some(data)})
    | TopWordsLoaded(data) =>
      ReasonReact.Update({...state, topwords: Some(data)})
    | ChangeQuery(text) => ReasonReact.Update({...state, query: text})
    },
  didMount: ({send}) => {
    Js.Promise.(
      Fetch.fetch("/data/mthesaur.txt")
      |> then_(Fetch.Response.text)
      |> then_(text => {
           let lines = Js.String.split("\n", text);
           let words = Array.map(line => Js.String.split(",", line), lines);
           let dict = Js.Dict.empty();
           Array.iter(
             word_line => {
               let head = word_line[0];
               let tail =
                 Array.sub(word_line, 1, Array.length(word_line) - 1);
               Js.Dict.set(dict, head, tail);
             },
             words,
           );
           send(DataLoaded(dict)) |> resolve;
         })
    )
    |> ignore;
    Js.Promise.(
      Fetch.fetch("/data/words1000.txt")
      |> then_(Fetch.Response.text)
      |> then_(text => {
           let lines = Js.String.split("\n", text);
           let wordset = StringSet.of_list(Array.to_list(lines));
           send(TopWordsLoaded(wordset));
           resolve();
         })
    )
    |> ignore;
  },
  render: ({send, state}) =>
    switch (state.thesaurus) {
    | None => <div> {ReasonReact.string("Loading...")} </div>
    | Some(data) =>
      let words = Js.Dict.get(data, state.query);
      <div className="cf">
        <div className="fl w-third measure">
          /* <label> should have for="" but Reason does not allow it. */

            <label className="f6 b db mb2">
              {ReasonReact.string("What are you looking for?")}
            </label>
            <input
              className="input-reset ba b--black-20 mb2 pa2 db w-100"
              placeholder="gimme da gorbage"
              value={state.query}
              autoFocus=true
              onChange=(
                event =>
                  send(ChangeQuery(ReactEvent.Form.target(event)##value))
              )
            />
          </div>
        <div className="fl f3 w-two-thirds ph4 lh-copy">
          {
            switch (words) {
            | None => ReasonReact.string("No synonyms found!")
            | Some(synonyms) =>
              switch (state.topwords) {
              | None => ReasonReact.string(Js.Array.joinWith(", ", synonyms))
              | Some(topwords) =>
                let filtered =
                  Js.Array.filter(
                    word => StringSet.mem(word, topwords),
                    synonyms,
                  );
                ReasonReact.string(Js.Array.joinWith(", ", filtered));
              }
            }
          }
        </div>
      </div>;
    },
};