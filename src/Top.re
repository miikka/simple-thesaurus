type action =
  | DataLoaded(Js.Dict.t(array(string)))
  | ChangeQuery(string);

type state = {
  thesaurus: option(Js.Dict.t(array(string))),
  query: string,
};

let component = ReasonReact.reducerComponent("Top");

external perfectlySafeCoerce: 'a => 'b = "%identity";

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
      Fetch.fetch("/data/thesaurus.json")
      |> then_(Fetch.Response.json)
      |> then_(dict =>
           send(DataLoaded(perfectlySafeCoerce(dict))) |> resolve
         )
    )
    |> ignore,
  render: ({send, state}) =>
    switch (state.thesaurus) {
    | None => <div> {ReasonReact.string("Loading...")} </div>
    | Some(data) =>
      let words = Js.Dict.get(data, state.query);
      <>
        <div className="fl w-third-ns w-100 measure">
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
        <div className="fl f3 w-two-thirds-ns w-100 ph4 lh-copy">
          {
            switch (words) {
            | None =>
              <span className="black-60">
                {ReasonReact.string("No synonyms found!")}
              </span>
            | Some(synonyms) =>
              ReasonReact.string(Js.Array.joinWith(", ", synonyms))
            }
          }
        </div>
      </>;
    },
};