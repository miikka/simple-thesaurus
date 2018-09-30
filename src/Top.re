type action =
  | DataLoaded(Js.Dict.t(array(string)))
  | ChangeQuery(string);

type state = {
  thesaurus: option(Js.Dict.t(array(string))),
  query: string,
};

let component = ReasonReact.reducerComponent("Top");

let make = _children => {
  ...component,
  initialState: () => {query: "", thesaurus: None},
  reducer: (action, state) =>
    switch (action) {
    | DataLoaded(data) =>
      ReasonReact.Update({...state, thesaurus: Some(data)})
    | ChangeQuery(text) => ReasonReact.Update({...state, query: text})
    },
  didMount: ({send}) =>
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
    |> ignore,
  render: ({send, state}) =>
    switch (state.thesaurus) {
    | None => <div> {ReasonReact.string("Loading...")} </div>
    | Some(data) =>
      let words = Js.Dict.get(data, state.query);
      <div>
        <input
          placeholder="gimme da gorbage"
          value={state.query}
          autoFocus=true
          onChange=(
            event => send(ChangeQuery(ReactEvent.Form.target(event)##value))
          )
        />
        {
          switch (words) {
          | None => ReasonReact.string("No synonyms found.")
          | Some(synonyms) =>
            ReasonReact.string(
              Js.Array.joinWith(", ", synonyms)
            )
          }
        }
      </div>;
    },
};